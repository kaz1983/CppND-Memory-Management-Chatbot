#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <wx/bitmap.h>
#include <string>

class GraphNode;
class ChatLogic;

class ChatBot
{
private:
    std::string _filename;
    wxBitmap *_image;

    GraphNode *_currentNode;
    GraphNode *_rootNode;
    ChatLogic *_chatLogic;

    int ComputeLevenshteinDistance(std::string s1, std::string s2);

public:
    ChatBot();
    ChatBot(std::string filename);
    ~ChatBot();

    // Task 2 - RULE OF FIVE (declaring copy constructor, copy assignment operator
    // move constructor, and move assignment operator)
    ChatBot(const ChatBot &source);
    ChatBot &operator=(const ChatBot &source);
    ChatBot(ChatBot &&source);
    ChatBot &operator=(ChatBot &&source);

    void SetCurrentNode(GraphNode *node);
    void SetRootNode(GraphNode *rootNode) { _rootNode = rootNode; }
    void SetChatLogicHandle(ChatLogic *chatLogic) { _chatLogic = chatLogic; }
    ChatLogic *GetChatLogicHandle() { return _chatLogic; }
    wxBitmap *GetImageHandle() { return _image; }

    void ReceiveMessageFromUser(std::string message);
};

#endif /* CHATBOT_H_ */
