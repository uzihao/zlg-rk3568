## 编译

源码：

[源码链接]([FFTW Download Page](http://fftw.org/download.html))

编译三部曲即可，简单

## 使用

FFTW基本用法，即如何计算单个阵列的傅里叶变换

### 计算大小为N的一维DFT

```c++
#include <fftw3.h>
...
{
    fftw_complex *in, *out;
    fftw_plan p;
    ...
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE); // FFTW_ESTIMATE与处理时间有关
	/* 计划创建时会修改in out，所以在此处修改in out */
    fftw_execute(p); /* repeat as needed */
    ...
    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
}
```

complex.h相关，但可以不用

### 多维DFT

