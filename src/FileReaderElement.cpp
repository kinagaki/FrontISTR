
#include "ElementType.h"

//
//	FileReaderElement.cpp
//
//				2008.04.20
//				2008.12.08
//				k.Takeda
#include "FileReaderElement.h"
using namespace FileIO;

//
//
CFileReaderElement::CFileReaderElement()
{
}

CFileReaderElement::~CFileReaderElement()
{
}


// Element
//
bool CFileReaderElement::Read(ifstream& ifs, string& sLine)
{
    string sTypeName;
    uint   nElementID, numOfElem, nMeshID, maxID, minID;
    vint   vLocalNode;
    uint   mgLevel(0);
    uint  i;

    //
    if(TagCheck(sLine, FileBlockName::StartElement()) ){// スタート タグ
        mpLogger->Info(Utility::LoggerMode::MWDebug, "FileReaderElement ", sLine);

        sLine = getLineSt(ifs);

        istringstream iss(sLine.c_str());
        iss >> numOfElem >> nMeshID >> maxID >> minID;

        mpFactory->reserveElement(mgLevel, nMeshID, numOfElem);
        mpFactory->initBucketElement(mgLevel, nMeshID, maxID, minID);

        uint nCount(0);// Index for mvElement
        //
        // 要素　読み込み -> Element生成, Bucketセットアップ(ID,Index)
        //
        while(true){
            sLine = getLineSt(ifs);
            if(TagCheck(sLine, FileBlockName::EndElement()) ) break; // エンド タグ

            istringstream iss(sLine.c_str());
            // iss -> Token 
            iss >> sTypeName;

            vLocalNode.clear();

            // 要素タイプ別処理
            //
            if(sTypeName=="Tetra"){
                vLocalNode.resize(4);

                // Element_ID, 局所ノードID, 局所ノードID, 局所ノードID, 局所ノードID
                iss >> nElementID;
                for(i=0; i< vLocalNode.size(); i++){ iss >> vLocalNode[i]; }
                
                // debug
                mpLogger->Monitor(Utility::LoggerMode::MWDebug, nElementID, vLocalNode, sTypeName);


                mpFactory->GeneElement(mgLevel, nMeshID, nElementID, pmw::ElementType::Tetra,vLocalNode);// Tetra
                mpFactory->setIDBucketElement(mgLevel, nMeshID, nElementID, nCount);//Bucket (ID & Index)
                nCount++;//次のループで、BucketへIndexをセットするためカウントアップ
            }
            if(sTypeName=="Hexa"){
                vLocalNode.resize(8);

                iss >> nElementID;
                for(i=0; i< vLocalNode.size(); i++){ iss >> vLocalNode[i]; }

                mpFactory->GeneElement(mgLevel, nMeshID, nElementID, pmw::ElementType::Hexa,vLocalNode);// Hexa
                mpFactory->setIDBucketElement(mgLevel, nMeshID, nElementID, nCount);
                nCount++;
            }
            if(sTypeName=="Prism"){
                vLocalNode.resize(6);

                iss >> nElementID;
                for(i=0; i< vLocalNode.size(); i++){ iss >> vLocalNode[i];}

                mpFactory->GeneElement(mgLevel, nMeshID, nElementID, pmw::ElementType::Prism,vLocalNode);// Prism
                mpFactory->setIDBucketElement(mgLevel, nMeshID, nElementID, nCount);
                nCount++;
            }
            if(sTypeName=="Pyramid"){
                vLocalNode.resize(5);

                iss >> nElementID;
                for(i=0; i< vLocalNode.size(); i++){ iss >> vLocalNode[i];}

                mpFactory->GeneElement(mgLevel, nMeshID, nElementID, pmw::ElementType::Pyramid,vLocalNode);//Pyramid
                mpFactory->setIDBucketElement(mgLevel, nMeshID, nElementID, nCount);
                nCount++;

                //Loggerによる警告
                mpLogger->Info(Utility::LoggerMode::Warn,"Non-Support MultiGrid, Pyramid Element");
            }
            if(sTypeName=="Triangle"){
                vLocalNode.resize(3);

                iss >> nElementID;
                for(i=0; i< vLocalNode.size(); i++){ iss >> vLocalNode[i];}

                mpFactory->GeneElement(mgLevel, nMeshID, nElementID, pmw::ElementType::Triangle,vLocalNode);// Triangle
                mpFactory->setIDBucketElement(mgLevel, nMeshID, nElementID, nCount);
                nCount++;
            }
            if(sTypeName=="Quad"){
                vLocalNode.resize(4);

                iss >> nElementID;
                for(i=0; i< vLocalNode.size(); i++){ iss >> vLocalNode[i]; }

                mpFactory->GeneElement(mgLevel, nMeshID, nElementID, pmw::ElementType::Quad,vLocalNode);// Quad
                mpFactory->setIDBucketElement(mgLevel, nMeshID, nElementID, nCount);
                nCount++;
            }
            if(sTypeName=="Beam"){
                vLocalNode.resize(2);

                iss >> nElementID;
                for(i=0; i< vLocalNode.size(); i++){ iss >> vLocalNode[i]; }

                mpFactory->GeneElement(mgLevel, nMeshID, nElementID, pmw::ElementType::Beam,vLocalNode);// Beam
                mpFactory->setIDBucketElement(mgLevel, nMeshID, nElementID, nCount);
                nCount++;
            }
        };
        // Bucket setup (All in-One 処理)
        mpFactory->setupBucketElement(mgLevel, nMeshID); // <= IDの管理から,Indexだけの管理へと移行のためIndexBucketはペンディング...
        mpFactory->setupElement(mgLevel, nMeshID);// <= CMesh::numOfElementの設定

        mpLogger->Info(Utility::LoggerMode::MWDebug, "FileReaderElement ", sLine);
        return true;
    }else{
        return false;
    }
}


