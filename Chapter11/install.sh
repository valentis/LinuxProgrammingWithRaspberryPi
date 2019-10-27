sudo apt-get update
sudo apt-get -y install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler cmake 
sudo apt-get -y install --no-install-recommends libboost-all-dev 
sudo apt-get -y install libatlas-base-dev libgflags-dev libgoogle-glog-dev liblmdb-dev

#install caffe
cd
git clone https://github.com/BVLC/caffe.git
cd caffe && mkdir build && cd build
cmake .. && make all && make install && make runtest
make runtest
make pycaffe
cd
echo 'export LD_LIBRARY_PATH=~/caffe/build/install/lib:$LD_LIBRARY_PATH' >> .bashrc
echo 'export PYTHONPATH=~/caffe/python/:$PYTHONPATH' >> .bashrc
source .bashrc

# training example
cd caffe
sudo apt-get install python-yaml
./scripts/download_model_binary.py models/bvlc_reference_caffenet
./data/ilsvrc12/get_ilsvrc_aux.sh
