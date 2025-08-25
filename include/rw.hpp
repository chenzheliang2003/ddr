#pragma once

#include <Eigen/Dense>
#include <filesystem>

void write(const Eigen::MatrixXd &matrix,const std::filesystem::path &path,char delimiter=' ');

Eigen::MatrixXd read(const std::filesystem::path &path,char delimiter=' ');

Eigen::MatrixXd read_csv(const std::filesystem::path &filename);

void write_csv(const Eigen::MatrixXd &matrix,const std::filesystem::path &filename);