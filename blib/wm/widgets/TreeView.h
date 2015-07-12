#pragma once

#include <blib/wm/widgets/Widget.h>
#include <blib/util/Tree.h>
#include <blib/linq.h>

namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			class TreeView : public Widget
			{
			public:
				class TreeNode : public blib::util::DynTree<TreeNode>
				{
				public:
					TreeNode* parent;
					std::string text;
					bool opened;
					int level;

					TreeNode()
					{
						parent = NULL;
						opened = false;
						text = "";
						level = 0;
					}
					virtual ~TreeNode()
					{
						blib::linq::deleteall(children);
					}

					TreeNode(std::string text, TreeNode* parent)
					{
						this->text = text;
						opened = false;
						parent->add(this);
					}

					void add(TreeNode* node)
					{
						node->parent = this;
						children.push_back(node);
					}
					bool hasChildren() { return !children.empty();  }

					bool allOpened()
					{
						return (opened && (parent ? parent->allOpened() : true)) || !parent;
					}

					bool isVisible()
					{
						if (parent == NULL)
							return true;
						if (!parent->isVisible())
							return false;
						if (hasChildren() && parent->allOpened())
							return true;
						return parent->allOpened();
					}

					TreeNode* getNode(std::string text)
					{
						for (std::list<TreeNode*>::iterator it = children.begin(); it != children.end(); it++)
							if ((*it)->text == text)
								return *it;
						return NULL;
					}

				};


				TreeNode* root;
				std::vector<std::pair<std::string, TreeNode*> > currentList;
				int scrollPosition;
				int selectedItem;

				TreeView();
				virtual void draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const;
				void buildList();
			};

		}
	}
}