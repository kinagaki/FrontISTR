//
//  FileReaderRefine.cpp
//
//                  2009.04.20
//                  2009.04.08
//                  k.Takeda
#include "FileReaderRefine.h"
using namespace FileIO;

// 
//
CFileReaderRefine::CFileReaderRefine()
{
    ;
}

CFileReaderRefine::~CFileReaderRefine()
{
    ;
}

// Refine Block
//
bool CFileReaderRefine::Read(ifstream& ifs, string& sLine)
{
    uint numOfLevel;// MultiGrid Level数

    if(TagCheck(sLine, FileBlockName::StartRefine()) ){
        //mpLogger->Info(Utility::LoggerMode::MWDebug, "FileReaderRefin ", sLine);

        // Refine数::マルチグリッド・レベル
        while(true){
            sLine = getLineSt(ifs);
            if(TagCheck(sLine, FileBlockName::EndRefine()) ) break;

            istringstream iss(sLine.c_str());
            // iss -> Token 
            iss >> numOfLevel;

            mpLogger->Info(Utility::LoggerMode::MWDebug, "FileReaderRefine::numOfLevel == ", numOfLevel);
        };

        // AssyModel  generate
        //
        mpFactory->GeneAssyModel(numOfLevel+1);//マルチグリッド階層数に合わせてAssyModelを生成
        mpFactory->setMGLevel(numOfLevel);     //マルチグリッド階層数のセット

        //mpLogger->Info(Utility::LoggerMode::MWDebug, "FileReaderRefin ", sLine);

        return true;
    }else{
        return false;
    }
}