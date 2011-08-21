cmake_minimum_required(VERSION 2.6)
project(EWSRAYTRACER)

#There are lots of scripts with cmake for finding external libraries. 
#See /usr/local/share/cmake-2.6/Modules/Find*.cmake for more examples
find_package(SDL)
find_package(OpenGL)

set(CC "clang")
set(CMAKE_CXX_FLAGS "-Wall -pipe -O2 -g")

set (SRC_FILES main.cpp GUI/SDLApp.cpp 
     Math/Point2D.cpp Math/Point3D.cpp Math/Ray.cpp Math/Vector3D.cpp Math/Roots3And4.cpp  Math/Maths.cpp Math/Matrix.cpp
     Utility/Animation.cpp Utility/Color.cpp Utility/ChunkParser.cpp Utility/ShadeRecord.cpp Storage/Grid.cpp Storage/KdTree.cpp Utility/SDL_Utility.cpp Utility/MedianFilter.cpp Storage/Storage.cpp
     Parser/Hash.cpp Parser/Parser.cpp Parser/Tokenizer.cpp Parser/Value.cpp 
     Tracer/RayCast.cpp
     Samplers/MultiJittered.cpp Samplers/Regular.cpp Samplers/Sampler.cpp 
     Cameras/Camera.cpp Cameras/Pinhole.cpp Cameras/ThinLens.cpp 
     Lights/Ambient.cpp Lights/AmbientOccluder.cpp Lights/AreaLight.cpp Lights/LightManager.cpp Lights/PositionLight.cpp Lights/Direction.cpp Lights/Environment.cpp Lights/SpotLight.cpp
     BRDFs/BRDF.cpp BRDFs/GlossySpecular.cpp BRDFs/Lambertian.cpp BRDFs/PerfectSpecular.cpp 
     Materials/Material.cpp Materials/Emissive.cpp Materials/Matte.cpp Materials/Phong.cpp Materials/Atmosphere.cpp Materials/Reflective.cpp
     Textures/PlaneChecker.cpp Textures/Texture.cpp Textures/NoiseTexture.cpp Textures/SkyTexture.cpp Textures/WoodTexture.cpp 
     Textures/StarsTexture.cpp Textures/GraniteTexture.cpp Textures/MarbleTexture.cpp Textures/ImageTexture.cpp Textures/TInstance.cpp
     Noise/LatticeNoise.cpp Noise/LinearNoise.cpp Noise/CubicNoise.cpp 
     Geometry/GeometryManager.cpp Geometry/GeometryObject.cpp Geometry/Instance.cpp 
     Geometry/Plane.cpp Geometry/Rectangle.cpp Geometry/Sphere.cpp Geometry/Cylinder.cpp Geometry/Disk.cpp Geometry/Torus.cpp Geometry/BBox.cpp Geometry/Box.cpp Geometry/Annulus.cpp Geometry/Stars.cpp Geometry/StarBox.cpp
     Geometry/Compound/Compound.cpp Geometry/Compound/Wedge.cpp Geometry/Compound/WedgeRing.cpp 
     Geometry/Mesh/Mesh.cpp Geometry/Mesh/Triangle.cpp Geometry/Mesh/MeditParser.cpp Geometry/Mesh/MeshManager.cpp Geometry/Mesh/LightWaveParser.cpp Geometry/Mesh/M3DSParser.cpp Geometry/Mesh/GeoSphere.cpp)

add_executable(raytracer ${SRC_FILES})
include_directories(.)
target_link_libraries(raytracer SDL SDLmain SDL_image)
