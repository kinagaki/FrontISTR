//
//  FileReaderAssyModel.cpp
//
//              2009.04.08
//              2009.04.08
//              k.Takeda
#include "FileReaderAssyModel.h"
using namespace FileIO;

//  
//
CFileReaderAssyModel::CFileReaderAssyModel()
{
    ;
}

CFileReaderAssyModel::~CFileReaderAssyModel()
{
    ;
}

//
//
bool CFileReaderAssyModel::Read(ifstream& ifs, string& sLine)
{
    uint nMeshID, numOfMesh, maxID, minID;
    vint vMeshID;
    uint mgLevel(0);// mgLevel=0 ::ファイル入力時のマルチグリッド・レベルは恒等的に==0

    // MeshIDデータ for AssyModel
    if(TagCheck(sLine, FileBlockName::StartAssyModel()) ){
        mpLogger->Info(Utility::LoggerMode::MWDebug, "FileReaderAssyModel", sLine);

        // メッシュ数,MaxID,MinID
        //
        sLine = getLineSt(ifs);
        istringstream iss(sLine.c_str());
        iss >> numOfMesh >> maxID >> minID;
        
        // setup to BucketMesh in AssyModel
        mpFactory->setupBucketMesh(mgLevel, numOfMesh, maxID, minID);
        
        // MeshID の連続データ
        //
        while(true){
            sLine = getLineSt(ifs);
            if(TagCheck(sLine, FileBlockName::EndAssyModel()) ) break;

            istringstream iss(sLine.c_str());
            // iss -> Token 
            while(iss >> nMeshID){
                mpLogger->Info(Utility::LoggerMode::MWDebug, "nMeshID == ", nMeshID);

                vMeshID.push_back(nMeshID);
            };
        };
        // Meshの領域確保
        //
        mpFactory->reserveMesh(mgLevel, numOfMesh);//ファイル読み込みなので,mgLevel=0

        // Meshの生成 for AssyModel(at mgLevel=0)
        //
        uint i;
        for(i=0; i < vMeshID.size(); i++){
            mpFactory->GeneMesh(mgLevel, vMeshID[i], i);// ファイル読み込みなのでmgLevel=0
        };
        return true;
    }else{
        return false;
    }
}
