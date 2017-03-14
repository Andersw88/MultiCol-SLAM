echo "Configuring and building Thirdparty/DBoW2 ..."

# RelWithDebInfo
# Release
# Debug
buildType=RelWithDebInfo

cd ThirdParty/DBoW2
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=$buildType
make -j4

# echo $buildType

cd ../../
echo "Configuring and building Thirdparty/g2o ..."

EIGEN3_INCLUDE_DIR=$(pwd)/Eigen
export EIGEN3_INCLUDE_DIR

cd g2o
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=$buildType
make -j4

echo "Configuring and building Thirdparty/opengv ..."

cd ../../OpenGV

mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=$buildType
make -j4

echo "Configuring and building MultiCol-SLAM ..."
cd ../../../
echo $(pwd)
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=$buildType
make -j4