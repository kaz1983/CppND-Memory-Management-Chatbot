#ifndef GRAPHEDGE_H_
#define GRAPHEDGE_H_

#include <vector>
#include <string>

class GraphNode;

class GraphEdge
{
private:
    GraphNode *_childNode;
    GraphNode *_parentNode;

    int _id;
    std::vector<std::string> _keywords; 
    

public:
    GraphEdge(int id);

    int GetID() const { return _id; }
    void SetChildNode(GraphNode *childNode);
    void SetParentNode(GraphNode *parentNode);
    GraphNode *GetChildNode() const { return _childNode; }
    std::vector<std::string> GetKeywords() const { return _keywords; }

    void AddToken(std::string token);
};

#endif /* GRAPHEDGE_H_ */