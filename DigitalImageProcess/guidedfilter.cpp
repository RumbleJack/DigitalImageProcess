#include "stdafx.h"
#include "guidedfilter.h"
using std::vector;
using cv::Size;

//引导滤波器实现
class GuidedFilterImpl
{
public:
	virtual ~GuidedFilterImpl() {}
	Mat filter(const Mat &p);
protected:
	static Mat meanFilter(const Mat &I, int r)
	{
		Mat result;
		cv::blur(I, result, cv::Size(r*2+1, r*2+1));
		return result;
	}
protected:
	int Idepth;
private:
	virtual Mat filterSingleChannel(const Mat &p) const = 0;
};
Mat GuidedFilterImpl::filter(const Mat &p)
{
	Mat p2;
	p.convertTo(p2, Idepth);

	Mat result;
	if (p.channels() == 1)
		result = filterSingleChannel(p2);
	else{
		vector<Mat> pc;
		cv::split(p2, pc);

		for (std::size_t i = 0; i < pc.size(); ++i)
			pc[i] = filterSingleChannel(pc[i]);
		cv::merge(pc, result);
	}
	//还原输入图像p的位深
	result.convertTo(result, p.depth());
	return result;
}

//灰度引导图像滤波器
class GuidedFilterMono : public GuidedFilterImpl
{
public:
	GuidedFilterMono(const Mat &I, int r, double eps);

private:
	virtual Mat filterSingleChannel(const Mat &p) const override;

private:
	int r;
	double eps;
	Mat I, mean_I, var_I;
};
GuidedFilterMono::GuidedFilterMono(const Mat &I, int r, double eps) : I(I),r(r), eps(eps)
{
	Idepth = I.depth();
	mean_I = meanFilter(I, r);
	Mat mean_II = meanFilter(I.mul(I), r);
	var_I = mean_II - mean_I.mul(mean_I);
}
Mat GuidedFilterMono::filterSingleChannel(const Mat &p) const
{
	Mat mean_p = meanFilter(p, r);
	Mat mean_Ip = meanFilter(I.mul(p), r);
	Mat cov_Ip = mean_Ip - mean_I.mul(mean_p); // this is the covariance of (I, p) in each local patch.

	Mat a = cov_Ip / (var_I + eps); // Eqn. (5) in the paper;
	Mat b = mean_p - a.mul(mean_I); // Eqn. (6) in the paper;

	Mat mean_a = meanFilter(a, r);
	Mat mean_b = meanFilter(b, r);

	return mean_a.mul(I) + mean_b;
}

// 彩色引导图像滤波器
class GuidedFilterColor : public GuidedFilterImpl
{
public:
	GuidedFilterColor(const Mat &I, int r, double eps) ;
private:
	virtual Mat filterSingleChannel(const Mat &p) const override;
private:
	vector<Mat> Ichannels;
	int r;
	double eps;
	Mat mean_I_r, mean_I_g, mean_I_b, invrr, invrg, invrb, invgg, invgb, invbb;
};
GuidedFilterColor::GuidedFilterColor(const Mat &I, int r, double eps) : r(r), eps(eps)
{
	Idepth = I.depth();
	cv::split(I, Ichannels);

	//计算各颜色通道半径为r的均值滤波结果
	mean_I_r = meanFilter(Ichannels[0], r);
	mean_I_g = meanFilter(Ichannels[1], r);
	mean_I_b = meanFilter(Ichannels[2], r);

	// variance of I in each local patch: the matrix Sigma in Eqn (14).
	// Note the variance in each local patch is a 3x3 symmetric matrix:
	//           rr, rg, rb
	//   Sigma = rg, gg, gb
	//           rb, gb, bb
	//向量协方差矩阵中的每个元素对应，向量中每个随机变量之间的协方差
	//（协）方差等于均方值减去均值的平方
	Mat var_I_rr = meanFilter(Ichannels[0].mul(Ichannels[0]), r) - mean_I_r.mul(mean_I_r) + eps;
	Mat var_I_gg = meanFilter(Ichannels[1].mul(Ichannels[1]), r) - mean_I_g.mul(mean_I_g) + eps;
	Mat var_I_bb = meanFilter(Ichannels[2].mul(Ichannels[2]), r) - mean_I_b.mul(mean_I_b) + eps;
	Mat var_I_rg = meanFilter(Ichannels[0].mul(Ichannels[1]), r) - mean_I_r.mul(mean_I_g);
	Mat var_I_rb = meanFilter(Ichannels[0].mul(Ichannels[2]), r) - mean_I_r.mul(mean_I_b);
	Mat var_I_gb = meanFilter(Ichannels[1].mul(Ichannels[2]), r) - mean_I_g.mul(mean_I_b);

	// Inverse of Sigma + eps * I
	// 求解伴随矩阵
	invrr = var_I_gg.mul(var_I_bb) - var_I_gb.mul(var_I_gb);
	invrg = var_I_gb.mul(var_I_rb) - var_I_rg.mul(var_I_bb);
	invrb = var_I_rg.mul(var_I_gb) - var_I_gg.mul(var_I_rb);
	invgg = var_I_rr.mul(var_I_bb) - var_I_rb.mul(var_I_rb);
	invgb = var_I_rb.mul(var_I_rg) - var_I_rr.mul(var_I_gb);
	invbb = var_I_rr.mul(var_I_gg) - var_I_rg.mul(var_I_rg);
	// 求解Sigma + eps * I矩阵的行列式
	Mat covDet = invrr.mul(var_I_rr) + invrg.mul(var_I_rg) + invrb.mul(var_I_rb);

	// 求解Inverse of Sigma + eps * I
	invrr /= covDet;
	invrg /= covDet;
	invrb /= covDet;
	invgg /= covDet;
	invgb /= covDet;
	invbb /= covDet;
}
Mat GuidedFilterColor::filterSingleChannel(const Mat &p) const
{
	Mat mean_p = meanFilter(p, r);

	Mat mean_Ip_r = meanFilter(Ichannels[0].mul(p), r);
	Mat mean_Ip_g = meanFilter(Ichannels[1].mul(p), r);
	Mat mean_Ip_b = meanFilter(Ichannels[2].mul(p), r);

	// covariance of (I, p) in each local patch.求解I每个颜色通道和p在一个局部的协方差
	Mat cov_Ip_r = mean_Ip_r - mean_I_r.mul(mean_p);
	Mat cov_Ip_g = mean_Ip_g - mean_I_g.mul(mean_p);
	Mat cov_Ip_b = mean_Ip_b - mean_I_b.mul(mean_p);

	//求解系数a和b
	Mat a_r = invrr.mul(cov_Ip_r) + invrg.mul(cov_Ip_g) + invrb.mul(cov_Ip_b);
	Mat a_g = invrg.mul(cov_Ip_r) + invgg.mul(cov_Ip_g) + invgb.mul(cov_Ip_b);
	Mat a_b = invrb.mul(cov_Ip_r) + invgb.mul(cov_Ip_g) + invbb.mul(cov_Ip_b);

	Mat b = mean_p - a_r.mul(mean_I_r) - a_g.mul(mean_I_g) - a_b.mul(mean_I_b); // Eqn. (15) in the paper;
	//得到滤波结果
	return (meanFilter(a_r, r).mul(Ichannels[0])
		+ meanFilter(a_g, r).mul(Ichannels[1])
		+ meanFilter(a_b, r).mul(Ichannels[2])
		+ meanFilter(b, r));  // Eqn. (16) in the paper;
}

//引导滤波器
GuidedFilter::GuidedFilter(const Mat &I, int r, double eps)
{
	//正规化引导图像位深
	Mat NormalI;
	if (I.depth() == CV_32F || I.depth() == CV_64F)
		NormalI = I.clone();
	else
		I.convertTo(NormalI, CV_32F);
	//判断引导图像通道数
	CV_Assert(I.channels() == 1 || I.channels() == 3);
	if (I.channels() == 1)
		impl_ = new GuidedFilterMono(NormalI,  r , eps);
	else
		impl_ = new GuidedFilterColor(NormalI, r , eps);
}
GuidedFilter::~GuidedFilter()
{
	delete impl_;
}
Mat GuidedFilter::filter(const Mat &p) const
{
	return impl_->filter(p);
}

//定义引导滤波器函数接口
Mat guidedFilter(const Mat &I, const Mat &p, int r, double eps)
{
	return GuidedFilter(I, r, eps).filter(p);
}
