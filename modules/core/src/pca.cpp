#include "pca.hpp"

PCA::PCA():
svd_option(THIN){}

void PCA::set_svd_option(SVDOption value)
{
    svd_option=value;
}

void PCA::fit(const Eigen::MatrixXd &data,double eps)
{
    mean=data.colwise().mean();
    Eigen::MatrixXd centered=data.rowwise()-mean;
    Eigen::JacobiSVD<Eigen::MatrixXd> svd
    (
        centered,
        svd_option==THIN?Eigen::ComputeThinU|Eigen::ComputeThinV
                        :Eigen::ComputeFullU|Eigen::ComputeFullV
    );
    const Eigen::VectorXd &s=svd.singularValues();
    double total_var=s.squaredNorm();
    double thres=total_var*(1.0-eps);
    double current_var=0.0;
    int i=0;
    while(i<s.size()&&current_var<thres)
    {
        current_var+=s[i]*s[i];
        ++i;
    }
    components=svd.matrixV().leftCols(i);
    explained=current_var/total_var;
}

void PCA::fit(const Eigen::MatrixXd &data,int ndimension)
{
    mean=data.colwise().mean();
    Eigen::MatrixXd centered=data.rowwise()-mean;
    Eigen::JacobiSVD<Eigen::MatrixXd> svd
    (
        centered,
        svd_option==THIN?Eigen::ComputeThinU|Eigen::ComputeThinV
                        :Eigen::ComputeFullU|Eigen::ComputeFullV
    );
    const Eigen::VectorXd &s=svd.singularValues();
    ndimension=std::min(ndimension,static_cast<int>(s.size()));
    components=svd.matrixV().leftCols(ndimension);
    double current_var=0.0;
    for(int i=0;i<ndimension;++i)
        current_var+=s[i]*s[i];
    explained=current_var/s.squaredNorm();
}

Eigen::MatrixXd PCA::transform(const Eigen::MatrixXd &x) const
{
    return (x.rowwise()-mean)*components;
}

Eigen::MatrixXd PCA::reconstruct(const Eigen::MatrixXd &x) const
{
    return (x*components.transpose()).rowwise()+mean;
}

double PCA::get_explained() const
{
    return explained;
}