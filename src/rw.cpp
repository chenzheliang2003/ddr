#include <fstream>
#include "rw.hpp"

void write(const Eigen::MatrixXd &matrix,const std::filesystem::path &filename,char delimiter)
{
    std::ofstream outfile(filename);
    if(!outfile.is_open())
        throw std::runtime_error("Failed to open file: "+filename.string());
    outfile.precision(17);
    for(int i=0;i<matrix.rows();++i)
    {
        for(int j=0;j<matrix.cols();++j)
        {
            outfile<<matrix(i,j);
            if(j<matrix.cols()-1)
                outfile<<delimiter;
        }
        outfile<<std::endl;
    }
}

Eigen::MatrixXd read(const std::filesystem::path &filename,char delimiter)
{
    std::ifstream infile(filename);
    if (!infile.is_open())
        throw std::runtime_error("Failed to open file: "+filename.string());
    std::vector<std::vector<double>> data;
    std::string line;
    while(std::getline(infile,line))
    {
        if(line.empty()||line[0]=='#')
            continue;
        std::vector<double> row;
        std::istringstream iss(line);
        std::string value;
        while(std::getline(iss,value,delimiter))
            try
            {
                value.erase(std::remove_if(value.begin(),value.end(),::isspace),value.end());
                if(!value.empty())
                    row.push_back(std::stod(value));
            }
            catch(const std::exception &e)
            {
                throw std::runtime_error("Error parsing value: "+value+" in file: "+filename.string());
            }
        if(!row.empty())
            data.push_back(row);
    }
    if(data.empty())
        return Eigen::MatrixXd(0,0);
    const size_t cols=data[0].size();
    for(size_t i=1;i<data.size();++i)
        if(data[i].size()!=cols)
            throw std::runtime_error("Inconsistent column count in file: "+filename.string());
    Eigen::MatrixXd matrix(data.size(),cols);
    for(size_t i=0;i<data.size();++i)
        for(size_t j=0;j<cols;++j)
            matrix(i,j)=data[i][j];
    return matrix;
}

Eigen::MatrixXd read_csv(const std::filesystem::path &filename)
{
    return read(filename,',');
}

void write_csv(const Eigen::MatrixXd &matrix,const std::filesystem::path &filename)
{
    write(matrix,filename,',');
}