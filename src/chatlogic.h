#ifndef CHATLOGIC_H_
#define CHATLOGIC_H_

#include <memory>
#include <vector>
#include <string>
#include "chatgui.h"

class ChatBot;
class GraphEdge;
class GraphNode;

class ChatLogic
{
private:
    // Task 3 - replaced std::vector<GraphNode *> _nodes with vector of unique_ptr
    // so that elements of GraphNodels are exclusively owned by ChatLogic class.
    std::vector<std::unique_ptr<GraphNode>> _nodes;

    // Task 4 - removed std::vector<GraphEdge *> _edges since edge information
    // is already tracked for the nodes.

    GraphNode *_currentNode;
    ChatBot *_chatBot;
    ChatBotPanelDialog *_panelDialog;

    typedef std::vector<std::pair<std::string, std::string>> tokenlist;

    template <typename T>
    void AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element);

public:
    ChatLogic();
    ~ChatLogic();

    void SetPanelDialogHandle(ChatBotPanelDialog *panelDialog);
    void SetChatbotHandle(ChatBot *chatbot);

    void LoadAnswerGraphFromFile(std::string filename);
    void SendMessageToChatbot(std::string message);
    void SendMessageToUser(std::string message);
    wxBitmap *GetImageFromChatbot();
};

#endif /* CHATLOGIC_H_ */