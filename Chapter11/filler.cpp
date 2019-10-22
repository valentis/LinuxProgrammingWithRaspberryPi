#include <caffe/caffe.hpp>

using namespace std;
using namespace caffe;

int main(int argc, char** argv)
{
    Blob<double> blob(20, 30, 40, 50); /* Blob 클래스의 매개변수를 설정 */

    FillerParameter filler_param; 		/* 필러의 매개변수 설정 */
    filler_param.set_min(-3); 			/* 최솟값: -3 */
    filler_param.set_max(3); 			/* 최댓값: 3 */

    UniformFiller<double> filler(filler_param); /* UniformFiller 설정 */
    filler.Fill(&blob); 			/* Blob 클래스의 객체를 초기화 */

    /* Blob의 합의 절댓값 */
    double expected_asum = 0;
    const double* data = blob.cpu_data();
    for (int i = 0; i < blob.count(); i++) {
        expected_asum += fabs(data[i]);
    }

    cout << "expected asum of BLOB : " << expected_asum << endl;
    cout << "asum of BLOB on CPU : " << blob.asum_data() << endl;

    return 0;
}
