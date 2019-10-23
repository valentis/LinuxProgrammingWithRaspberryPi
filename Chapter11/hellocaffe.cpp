#include <caffe/caffe.hpp>

using namespace std;
using namespace caffe;

int main(int argc, char** argv)
{
    cout << "Hello, Caffe!" << endl;

    Blob<double> blob(20, 30, 40, 50); 		/* Blob 클래스의 매개변수를 설정 */
    cout << "Size of BLOB" << endl;
    cout << " Num : " << blob.num() << endl;
    cout << " Channels : " << blob.channels() << endl;
    cout << " Height : " << blob.height() << endl;
    cout << " Width : " << blob.width() << endl;
    cout << " Count : " << blob.count() << endl;

    return 0;
}
