#include <caffe/caffe.hpp>

using namespace std;
using namespace caffe;

int main(int argc, char** argv)
{
    clock_t tStart, tEnd; 			/* 시간 측정을 위한 변수 */

    Blob<double> blob(20, 30, 40, 50); 		/* Blob 클래스의 매개변수를 설정 */

    blob.Reshape(50, 40, 30, 20); 		/* Blob의 값을 재설정 */
    cout << "Size of Reshaped BLOB" << endl;
    cout << " Num : " << blob.num() << endl;
    cout << " Channels : " << blob.channels() << endl;
    cout << " Height : " << blob.height() << endl;
    cout << " Width : " << blob.width() << endl;
    cout << " Count : " << blob.count() << endl;

    FillerParameter filler_param; 		/* 필러의 매개변수 설정 */
    filler_param.set_min(-3); 			/* 최솟값: -3 */
    filler_param.set_max(3); 			/* 최댓값: 3 */
    UniformFiller<double> filler(filler_param); /* UniformFiller 설정 */
    filler.Fill(&blob); 			/* Blob 초기화 */
    
    /* Blob 합의 절댓값 */
    double expected_asum = 0;
    const double* data = blob.cpu_data();
    for (int i = 0; i < blob.count(); i++)
        expected_asum += fabs(data[i]);

    cout << "expected asum of BLOB : " << expected_asum << endl;
    tStart = clock();
    cout << "asum of BLOB on CPU : " << blob.asum_data() << endl;
    tEnd = clock();
    cout << "Time of asum of Blob on cpu : " << (tEnd-tStart)/CLOCKS_PER_SEC << endl;
    
    return 0;
}
