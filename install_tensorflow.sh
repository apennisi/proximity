#!/bin/bash
echo "INSTALLING TENSORFLOW.."

current_dir="$PWD"

cd ~/
wget https://github.com/bazelbuild/bazel/releases/download/0.25.2/bazel-0.25.2-installer-linux-x86_64.sh
chmod +x bazel-0.25.2-installer-linux-x86_64.sh
./bazel-0.25.2-installer-linux-x86_64.sh --user
echo "export $PATH=$PATH:~/bin" >> ~/.bashrc
source ~/.bashrc
rm -rf ~/.cache/
rm bazel-0.25.2-installer-linux-x86_64.sh

cd "$current_dir"
sudo apt-get install cmake curl g++-7 git python3-dev python3-numpy wget
git clone https://github.com/FloopCZ/tensorflow_cc.git
cd tensorflow_cc/tensorflow_cc

sed -i 's/v2.2.0-rc2/v1.14.0/g' CMakeLists.txt
mkdir build
cd build
cmake ..
make && (
  echo "TENSORFLOW COMPILED!"
) || (
  echo "ERROR IN COMPILING TENSORFLOW, I AM GOING TO FIX IT AND RELAUNCH THE MAKE COMMAND!"
  sed -i 's/"--bin2c-path=%s" % bin2c.dirname,//g' tensorflow/third_party/nccl/build_defs.bzl.tpl
  make
)

echo "TENSORFLOW COMPILED"

sudo make install

echo "TENSORFLOW INSTALLED"

sudo rm -rf /usr/local/lib/cmake/TensorflowCC/TensorflowCCTargets.cmake
sudo cp tensorflow_files/TensorflowCCTargets.cmake /usr/local/lib/cmake/TensorflowCC/
