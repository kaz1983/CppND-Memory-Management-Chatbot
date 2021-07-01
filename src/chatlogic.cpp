#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <tuple>
#include <algorithm>

#include "graphedge.h"
#include "graphnode.h"
#include "chatbot.h"
#include "chatlogic.h"


ChatLogic::ChatLogic()
{
    // Task 5 - Comment out the ChatBot instance so that there is no ownership relationship with the ChatLogic 
    //_chatBot = new ChatBot("../images/chatbot.png")

    //
    //_chatBot->SetChatLogicHandle(this);
}

ChatLogic::~ChatLogic()
{
    // Task 5 - No need to manage memory since _chatBot is not owned by ChatLogic
    // delete chatbot instance
    //delete _chatBot;

    // Task 3 - No need to manage memory since _nodes is a unique_ptr
    // delete all nodes
    //for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    //{
        //delete *it;
    //}

    // Task 4 - No need to manage memory since _edges is a unique_ptr
    // delete all edges
    //for (auto it = std::begin(_edges); it != std::end(_edges); ++it)
    //{
        //delete *it;
    //}
}

template <typename T>
void ChatLogic::AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element)
{
    auto token = tokens.begin();
    while (true)
    {
        token = std::find_if(token, tokens.end(), [&tokenID](const std::pair<std::string, std::string> &pair) { return pair.first == tokenID;; });
        if (token != tokens.end())
        {
            element.AddToken(token->second); 
            token++;                        
        }
        else
        {
            break; 
        }
    }
}

void ChatLogic::LoadAnswerGraphFromFile(std::string filename)
{
    
    std::ifstream file(filename);

    if (file)
    {
        std::string lineStr;
        while (getline(file, lineStr))
        {
            tokenlist tokens;
            while (lineStr.size() > 0)
            {
                int posTokenFront = lineStr.find("<");
                int posTokenBack = lineStr.find(">");
                if (posTokenFront < 0 || posTokenBack < 0)
                    break; 
                std::string tokenStr = lineStr.substr(posTokenFront + 1, posTokenBack - 1);

                int posTokenInfo = tokenStr.find(":");
                if (posTokenInfo != std::string::npos)
                {
                    std::string tokenType = tokenStr.substr(0, posTokenInfo);
                    std::string tokenInfo = tokenStr.substr(posTokenInfo + 1, tokenStr.size() - 1);

                    tokens.push_back(std::make_pair(tokenType, tokenInfo));
                }

                lineStr = lineStr.substr(posTokenBack + 1, lineStr.size());
            }

            auto type = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "TYPE"; });
            if (type != tokens.end())
            {
                auto idToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "ID"; });
                if (idToken != tokens.end())
                {

                    int id = std::stoi(idToken->second);

                    if (type->second == "NODE")
                    {
                        //Task 3 - changed raw pointer node to unique_ptr
                        auto newNode = std::find_if(_nodes.begin(), _nodes.end(), [&id](const std::unique_ptr<GraphNode> &node) { return node->GetID() == id; });

                        if (newNode == _nodes.end())
                        {
                            _nodes.emplace_back(std::make_unique<GraphNode>(id));
                            newNode = _nodes.end() - 1; 

                            AddAllTokensToElement("ANSWER", tokens, **newNode);
                        }

                    }

                    if (type->second == "EDGE")
                    {
                        
                        auto parentToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "PARENT"; });
                        auto childToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "CHILD"; });

                        if (parentToken != tokens.end() && childToken != tokens.end())
                        {
                            
                            auto parentNode = std::find_if(_nodes.begin(), _nodes.end(), [&parentToken](const std::unique_ptr<GraphNode> &node) { return node->GetID() == std::stoi(parentToken->second); });
                            auto childNode = std::find_if(_nodes.begin(), _nodes.end(), [&childToken](const std::unique_ptr<GraphNode> &node) { return node->GetID() == std::stoi(childToken->second); });

                            // Task 4 - created a unique_ptr for graph edge.
                            // Move ownership to child nodes 
                            std::unique_ptr<GraphEdge> edge = std::make_unique<GraphEdge>(id);
                            edge->SetChildNode(childNode->get());
                            edge->SetParentNode(parentNode->get());
                            
                            AddAllTokensToElement("KEYWORD", tokens, *edge);
                            
                            (*childNode)->AddEdgeToParentNode(edge.get());
                            (*parentNode)->AddEdgeToChildNode(std::move(edge));
                        }
                    }
                }
                else
                {
                    std::cout << "Error: ID missing. Line is ignored!" << std::endl;
                }
            }
        } 

        file.close();

    } 
    else
    {
        std::cout << "File could not be opened!" << std::endl;
        return;
    }

    GraphNode *rootNode = nullptr;
    for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    {
    
        if ((*it)->GetNumberOfParents() == 0)
        {

            if (rootNode == nullptr)
            {
                rootNode = it->get(); 
            }
            else
            {
                std::cout << "ERROR : Multiple root nodes detected" << std::endl;
            }
        }
    }

    // Task 5 - create local chatbot instance and move it thru the nodes
    ChatBot chatbot = ChatBot{"../images/chatbot.png"};
    chatbot.SetChatLogicHandle(this);

    chatbot.SetRootNode(rootNode);
    rootNode->MoveChatbotHere(std::move(chatbot));
}

void ChatLogic::SetPanelDialogHandle(ChatBotPanelDialog *panelDialog)
{
    _panelDialog = panelDialog;
}

void ChatLogic::SetChatbotHandle(ChatBot *chatbot)
{
    _chatBot = chatbot;
}

void ChatLogic::SendMessageToChatbot(std::string message)
{
    _chatBot->ReceiveMessageFromUser(message);
}

void ChatLogic::SendMessageToUser(std::string message)
{
    _panelDialog->PrintChatbotResponse(message);
}

wxBitmap *ChatLogic::GetImageFromChatbot()
{
    return _chatBot->GetImageHandle();
}
