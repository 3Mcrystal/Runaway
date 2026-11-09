#include "Serializer.h"
#include "pch.h"
#include <fstream>
#include <iostream>

json& Serializer::GetData()
{
    return m_data;
};

void Serializer::Save(const std::string& path)
{
    std::ofstream out(path);
    out << m_data.dump(4);
    out.close();
}

void Serializer::Load(const std::string& path)
{
    std::ifstream in(path);
    if (!in.is_open())
    {
        std::cout << "Error path doesn't exist" << std::endl;
        return;
    }

    m_data = json::parse(in);
    in.close();
}