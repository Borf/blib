# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := blib
LOCAL_SRC_FILES :=\
	./AnimatableSprite.cpp\
	./Animation.cpp\
	./Box2DDebug.cpp\
	./Color.cpp\
	./Font.cpp\
	./LineBatch.cpp\
	./linq.cpp\
	./Math.cpp\
	./Renderer.cpp\
	./RenderState.cpp\
	./Shader.cpp\
	./SpriteBatch.cpp\
	./SpriteSheet.cpp\
	./Util.cpp\
	./IWindow.cpp\
	./Window.cpp\
	./util/FileSystem.cpp\
	./util/Log.cpp\
	./util/Mutex.cpp\
	./util/Profiler.cpp\
	./util/Semaphore.cpp\
	./util/Signal.cpp\
	./util/stb_image.c\
	./util/Stream.cpp\
	./util/StreamIn.cpp\
	./util/StreamInFile.cpp\
	./util/StreamOut.cpp\
	./util/Thread.cpp\
	./gl/FBO.cpp\
	./gl/GlResizeRegister.cpp\
	./gl/Renderer.cpp\
	./gl/ResourceManager.cpp\
	./gl/Shader.cpp\
	./gl/TextureMap.cpp\
	./gl/Vertex.cpp\
	./gl/Window.cpp\
	./math/Line.cpp\
	./math/Polygon.cpp\
	./math/Random.cpp\
	./math/Ray.cpp\
	./math/Rectangle.cpp\
	./wm/Window.cpp\
	./wm/WM.cpp\
	./wm/widgets/button.cpp\
	./wm/widgets/ContainerWidget.cpp\
	./wm/widgets/Image.cpp\
	./wm/widgets/Label.cpp\
	./wm/widgets/list.cpp\
	./wm/widgets/Panel.cpp\
	./wm/widgets/ScrollPanel.cpp\
	./wm/widgets/textbox.cpp\
	./wm/widgets/Widget.cpp\
	./gl/Texture.cpp\
	./platform/android/Window.cpp\
	./App.cpp\
	./ParticleSystem.cpp
LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES) C:/Android/android-ndk-r9/platforms/android-14/arch-arm/usr/include $(LOCAL_PATH)/../ $(LOCAL_PATH)/../externals $(LOCAL_PATH)/../externals/box2d $(NDK_ROOT)/sources/android/native_app_glue

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)


