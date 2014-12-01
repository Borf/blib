TEMPLATE = lib
CONFIG += staticlib
CONFIG += threads
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG -= warn_on
CONFIG -= unicode

unix {
    CONFIG += object_with_source
 #   CONFIG += object_parallel_to_source
}

DEFINES -= UNICODE

INCLUDEPATH += .
INCLUDEPATH += externals
INCLUDEPATH += externals/box2d
windows {
    INCLUDEPATH += externals/glew/include
}

#QMAKE_EXT_CPP += .c
QMAKE_CXXFLAGS += -Wall -std=c++11
CONFIG += object_with_source
#CONFIG += object_parallel_to_source

SOURCES += \
    blib/Animation.cpp \
    blib/App.cpp \
    blib/BackgroundTask.cpp \
    blib/Box2DDebug.cpp \
    blib/Color.cpp \
    blib/Font.cpp \
    blib/IWindow.cpp \
    blib/KeyListener.cpp \
    blib/LineBatch.cpp \
    blib/linq.cpp \
    blib/Math.cpp \
    blib/ParticleSystem.cpp \
    blib/Renderer.cpp \
    blib/RenderState.cpp \
    blib/Shader.cpp \
    blib/Shapes.cpp \
    blib/SpriteBatch.cpp \
    blib/SpriteSheet.cpp \
    blib/Util.cpp \
    blib/Window.cpp \
	blib/json.cpp \
	blib/StaticModel.cpp \
	blib/util/ListAllocator.cpp \
    blib/gl/FBO.cpp \
    blib/gl/GlResizeRegister.cpp \
    blib/gl/Renderer.cpp \
    blib/gl/ResourceManager.cpp \
    blib/gl/Shader.cpp \
    blib/gl/Texture.cpp \
    blib/gl/TextureMap.cpp \
    blib/gl/Vertex.cpp \
    blib/gl/Window.cpp \
    blib/util/FileSystem.cpp \
    blib/util/Log.cpp \
    blib/util/Mutex.cpp \
    blib/util/Profiler.cpp \
    blib/util/Semaphore.cpp \
    blib/util/Signal.cpp \
    blib/util/Stream.cpp \
    blib/util/StreamIn.cpp \
    blib/util/StreamInFile.cpp \
    blib/util/Thread.cpp \
    blib/math/Line.cpp \
    blib/math/Polygon.cpp \
    blib/math/Random.cpp \
    blib/math/Rectangle.cpp \
    blib/wm/Menu.cpp \
    blib/wm/MenuItem.cpp \
    blib/wm/SubMenuMenuItem.cpp \
    blib/wm/Window.cpp \
    blib/wm/WM.cpp \
    blib/wm/widgets/button.cpp \
    blib/wm/widgets/ContainerWidget.cpp \
    blib/wm/widgets/Image.cpp \
    blib/wm/widgets/Label.cpp \
    blib/wm/widgets/list.cpp \
    blib/wm/widgets/Panel.cpp \
    blib/wm/widgets/ScrollPanel.cpp \
    blib/wm/widgets/textbox.cpp \
    blib/wm/widgets/Widget.cpp \
    externals/poly2tri/common/shapes.cc \
    externals/poly2tri/sweep/advancing_front.cc \
    externals/poly2tri/sweep/cdt.cc \
    externals/poly2tri/sweep/sweep.cc \
    externals/poly2tri/sweep/sweep_context.cc \
    externals/box2d/Box2D/Collision/b2BroadPhase.cpp \
    externals/box2d/Box2D/Collision/b2CollideCircle.cpp \
    externals/box2d/Box2D/Collision/b2CollideEdge.cpp \
    externals/box2d/Box2D/Collision/b2CollidePolygon.cpp \
    externals/box2d/Box2D/Collision/b2Collision.cpp \
    externals/box2d/Box2D/Collision/b2Distance.cpp \
    externals/box2d/Box2D/Collision/b2DynamicTree.cpp \
    externals/box2d/Box2D/Collision/b2TimeOfImpact.cpp \
    externals/box2d/Box2D/Collision/Shapes/b2ChainShape.cpp \
    externals/box2d/Box2D/Collision/Shapes/b2CircleShape.cpp \
    externals/box2d/Box2D/Collision/Shapes/b2EdgeShape.cpp \
    externals/box2d/Box2D/Collision/Shapes/b2PolygonShape.cpp \
    externals/box2d/Box2D/Common/b2BlockAllocator.cpp \
    externals/box2d/Box2D/Common/b2Draw.cpp \
    externals/box2d/Box2D/Common/b2Math.cpp \
    externals/box2d/Box2D/Common/b2Settings.cpp \
    externals/box2d/Box2D/Common/b2StackAllocator.cpp \
    externals/box2d/Box2D/Common/b2Timer.cpp \
    externals/box2d/Box2D/Dynamics/b2Body.cpp \
    externals/box2d/Box2D/Dynamics/b2ContactManager.cpp \
    externals/box2d/Box2D/Dynamics/b2Fixture.cpp \
    externals/box2d/Box2D/Dynamics/b2Island.cpp \
    externals/box2d/Box2D/Dynamics/b2World.cpp \
    externals/box2d/Box2D/Dynamics/b2WorldCallbacks.cpp \
    externals/box2d/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
    externals/box2d/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
    externals/box2d/Box2D/Dynamics/Contacts/b2CircleContact.cpp \
    externals/box2d/Box2D/Dynamics/Contacts/b2Contact.cpp \
    externals/box2d/Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
    externals/box2d/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
    externals/box2d/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
    externals/box2d/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
    externals/box2d/Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
    externals/box2d/Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
    externals/box2d/Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
    externals/box2d/Box2D/Dynamics/Joints/b2GearJoint.cpp \
    externals/box2d/Box2D/Dynamics/Joints/b2Joint.cpp \
    externals/box2d/Box2D/Dynamics/Joints/b2MouseJoint.cpp \
    externals/box2d/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
    externals/box2d/Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
    externals/box2d/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
    externals/box2d/Box2D/Dynamics/Joints/b2RopeJoint.cpp \
    externals/box2d/Box2D/Dynamics/Joints/b2WeldJoint.cpp \
    externals/box2d/Box2D/Dynamics/Joints/b2WheelJoint.cpp \
    externals/box2d/Box2D/Rope/b2Rope.cpp \
    blib/math/AABB.cpp \
    blib/math/ArcPath.cpp \
    blib/math/BiArc.cpp \
    blib/math/HermiteCurve.cpp \
    blib/math/Plane.cpp \
    blib/math/Quad.cpp \
    blib/math/Ray.cpp \
    blib/math/Triangle.cpp \
    blib/util/StreamOut.cpp \
    blib/util/stb_image.c

HEADERS += \
    blib/Animation.h \
    blib/App.h \
    blib/BackgroundTask.h \
    blib/blib.h \
    blib/Box2DDebug.h \
    blib/Color.h \
    blib/config.h \
    blib/FBO.h \
    blib/Font.h \
    blib/IDrawableLine.h \
    blib/IWindow.h \
    blib/JoyState.h \
    blib/KeyListener.h \
    blib/LineBatch.h \
    blib/linq.h \
    blib/MouseListener.h \
    blib/ParticleSystem.h \
    blib/Renderer.h \
    blib/RenderState.h \
    blib/ResourceManager.h \
    blib/Shader.h \
    blib/Shapes.h \
    blib/SpriteBatch.h \
    blib/SpriteSheet.h \
    blib/Texture.h \
    blib/TextureMap.h \
    blib/Util.h \
    blib/VBO.h \
    blib/VIO.h \
    blib/Window.h \
    blib/gl/Camera.h \
    blib/gl/FBO.h \
    blib/gl/GlResizeRegister.h \
    blib/gl/Renderer.h \
    blib/gl/ResourceManager.h \
    blib/gl/Shader.h \
    blib/gl/Texture.h \
    blib/gl/TextureMap.h \
    blib/gl/VAO.h \
    blib/gl/VBO.h \
    blib/gl/Vertex.h \
    blib/gl/VIO.h \
    blib/gl/Window.h \
    blib/util/Allocator.h \
    blib/util/FastDelegate.h \
    blib/util/FastDelegateBind.h \
    blib/util/FileSystem.h \
    blib/util/ListAllocator.h \
    blib/util/Log.h \
    blib/util/Mutex.h \
    blib/util/NotCopyable.h \
    blib/util/Profiler.h \
    blib/util/Semaphore.h \
    blib/util/Signal.h \
    blib/util/Singleton.h \
    blib/util/stb_image.h \
    blib/util/Stream.h \
    blib/util/StreamIn.h \
    blib/util/StreamInFile.h \
    blib/util/StreamOut.h \
    blib/util/StreamOutFile.h \
    blib/util/StreamReader.h \
    blib/util/StreamSeekable.h \
    blib/util/StreamWriter.h \
    blib/util/Thread.h \
    blib/math/Circle.h \
    blib/math/Line.h \
    blib/math/Plane.h \
    blib/math/Polygon.h \
    blib/math/Random.h \
    blib/math/Ray.h \
    blib/math/Rectangle.h \
    blib/platform/linux/config.h \
    blib/platform/linux/window.h \
    blib/wm/ActionMenuItem.h \
    blib/wm/Menu.h \
    blib/wm/MenuItem.h \
    blib/wm/SubMenuMenuItem.h \
    blib/wm/ToggleMenuItem.h \
    blib/wm/Window.h \
    blib/wm/WM.h \
    blib/wm/widgets/button.h \
    blib/wm/widgets/ContainerWidget.h \
    blib/wm/widgets/Image.h \
    blib/wm/widgets/Label.h \
    blib/wm/widgets/list.h \
    blib/wm/widgets/Panel.h \
    blib/wm/widgets/ScrollPanel.h \
    blib/wm/widgets/textbox.h \
    blib/wm/widgets/Widget.h \
    externals/poly2tri/poly2tri.h \
    externals/poly2tri/common/shapes.h \
    externals/poly2tri/common/utils.h \
    externals/poly2tri/sweep/advancing_front.h \
    externals/poly2tri/sweep/cdt.h \
    externals/poly2tri/sweep/sweep.h \
    externals/poly2tri/sweep/sweep_context.h \
    externals/box2d/Box2D/Box2D.h \
    externals/box2d/Box2D/Collision/b2BroadPhase.h \
    externals/box2d/Box2D/Collision/b2Collision.h \
    externals/box2d/Box2D/Collision/b2Distance.h \
    externals/box2d/Box2D/Collision/b2DynamicTree.h \
    externals/box2d/Box2D/Collision/b2TimeOfImpact.h \
    externals/box2d/Box2D/Collision/Shapes/b2ChainShape.h \
    externals/box2d/Box2D/Collision/Shapes/b2CircleShape.h \
    externals/box2d/Box2D/Collision/Shapes/b2EdgeShape.h \
    externals/box2d/Box2D/Collision/Shapes/b2PolygonShape.h \
    externals/box2d/Box2D/Collision/Shapes/b2Shape.h \
    externals/box2d/Box2D/Common/b2BlockAllocator.h \
    externals/box2d/Box2D/Common/b2Draw.h \
    externals/box2d/Box2D/Common/b2GrowableStack.h \
    externals/box2d/Box2D/Common/b2Math.h \
    externals/box2d/Box2D/Common/b2Settings.h \
    externals/box2d/Box2D/Common/b2StackAllocator.h \
    externals/box2d/Box2D/Common/b2Timer.h \
    externals/box2d/Box2D/Dynamics/b2Body.h \
    externals/box2d/Box2D/Dynamics/b2ContactManager.h \
    externals/box2d/Box2D/Dynamics/b2Fixture.h \
    externals/box2d/Box2D/Dynamics/b2Island.h \
    externals/box2d/Box2D/Dynamics/b2TimeStep.h \
    externals/box2d/Box2D/Dynamics/b2World.h \
    externals/box2d/Box2D/Dynamics/b2WorldCallbacks.h \
    externals/box2d/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.h \
    externals/box2d/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.h \
    externals/box2d/Box2D/Dynamics/Contacts/b2CircleContact.h \
    externals/box2d/Box2D/Dynamics/Contacts/b2Contact.h \
    externals/box2d/Box2D/Dynamics/Contacts/b2ContactSolver.h \
    externals/box2d/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.h \
    externals/box2d/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.h \
    externals/box2d/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h \
    externals/box2d/Box2D/Dynamics/Contacts/b2PolygonContact.h \
    externals/box2d/Box2D/Dynamics/Joints/b2DistanceJoint.h \
    externals/box2d/Box2D/Dynamics/Joints/b2FrictionJoint.h \
    externals/box2d/Box2D/Dynamics/Joints/b2GearJoint.h \
    externals/box2d/Box2D/Dynamics/Joints/b2Joint.h \
    externals/box2d/Box2D/Dynamics/Joints/b2MouseJoint.h \
    externals/box2d/Box2D/Dynamics/Joints/b2PrismaticJoint.h \
    externals/box2d/Box2D/Dynamics/Joints/b2PulleyJoint.h \
    externals/box2d/Box2D/Dynamics/Joints/b2RevoluteJoint.h \
    externals/box2d/Box2D/Dynamics/Joints/b2RopeJoint.h \
    externals/box2d/Box2D/Dynamics/Joints/b2WeldJoint.h \
    externals/box2d/Box2D/Dynamics/Joints/b2WheelJoint.h \
    externals/box2d/Box2D/Rope/b2Rope.h \
    blib/Math.h \
    blib/math/AABB.h \
    blib/math/ArcPath.h \
    blib/math/BiArc.h \
    blib/math/HermiteCurve.h \
    blib/math/Quad.h \
    blib/math/Triangle.h \
    blib/util/Tree.h

unix {
    SOURCES += blib/platform/linux/window.cpp

}

windows {
    SOURCES +=
    SOURCES +=
}
