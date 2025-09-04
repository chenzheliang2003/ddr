#pragma once

#include <Eigen/Dense>

class PCA
{
public:
    enum SVDOption
    {
        THIN,
        FULL
    };

public:
    PCA();

    void set_svd_option(SVDOption value);

    void fit(const Eigen::MatrixXd &x,double eps);

    void fit(const Eigen::MatrixXd &x,int ndimension);

    Eigen::MatrixXd transform(const Eigen::MatrixXd &x) const;

    Eigen::MatrixXd reconstruct(const Eigen::MatrixXd &x) const;

    double get_explained() const;

protected:
    SVDOption svd_option;
    Eigen::RowVectorXd mean;
    Eigen::MatrixXd components;
    double explained;
};