### line 

``` 从最开始的常数驱动的绘制精度，到根据实际坐标绘制（根据不同的斜率）决定从哪个方向开始绘制。最后进行的优化是根据像素来绘制的，通过一个斜率（也就是误差），来决定在绘制中y的偏移量。再次的改进是将float 变为int 的形式去加速计算和比较，  

int dx = x1 - x0;<br>
int dy = y1 - y0;<br>
int derror2 = std::abs(dy) * 2;<br>
int error2 = 0;//实际上就是直接将原来的斜率误差（和处理规则）等比放大
 ```