/* 
 * File:   NodeConnectNodeTree.h
 * Author: ktakeda
 *
 * Created on 2009/08/11, 19:06
 */
#ifndef _NODECONNECTNODETREE_H_209a9301_802f_414d_b5c4_c879ec87e1d1
#define	_NODECONNECTNODETREE_H_209a9301_802f_414d_b5c4_c879ec87e1d1

#include "CommonStd.h"
#include "TypeDef.h"

#include <iostream>//cout

namespace pmw{
class CNodeConnectNodeTree{
private:
    CNodeConnectNodeTree();
public:
    static CNodeConnectNodeTree* Instance(){
        static CNodeConnectNodeTree moNodeConnect;
        return &moNodeConnect;
    }
    virtual ~CNodeConnectNodeTree();

private:
    // Nodeの接続先ノード配列
    vvuint mvHexaConnectNode;
    vvuint mvTetraConnectNode;
    vvuint mvPrismConnectNode;
    vvuint mvPyramidConnectNode;
    vvuint mvQuadConnectNode;
    vvuint mvTriangleConnectNode;
    vvuint mvBeamConnectNode;

public:
    // 引数入力された局所番号の,接続先の局所番号列
    vuint& getHexaConnectNode(const uiint& localID){ return mvHexaConnectNode[localID];}
    vuint& getTetraConnectNode(const uiint& localID){ return mvTetraConnectNode[localID];}
    vuint& getPrismConnectNode(const uiint& localID){ return mvPrismConnectNode[localID];}
    vuint& getPyramidConnectNode(const uiint& localID){ return mvPyramidConnectNode[localID];}
    vuint& getQuadConnectNode(const uiint& localID){ return mvQuadConnectNode[localID];}
    vuint& getTriangleConnectNode(const uiint& localID){ return mvTriangleConnectNode[localID];}
    vuint& getBeamConnectNode(const uiint& localID){ return mvBeamConnectNode[localID];}
};
}
#endif	/* _NODECONNECTNODETREE_H */

