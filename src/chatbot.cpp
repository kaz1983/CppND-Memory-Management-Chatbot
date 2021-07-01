#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>

#include "chatlogic.h"
#include "graphnode.h"
#include "graphedge.h"
#include "chatbot.h"

ChatBot::ChatBot()
    : _image{ nullptr }
    , _currentNode{ nullptr }
    , _chatLogic{ nullptr }
    , _rootNode{ nullptr }
{}

ChatBot::ChatBot(std::string filename)
    : _filename{ filename }
    , _chatLogic{ nullptr }
    , _rootNode{ nullptr }
    , _currentNode{ nullptr }
    , _image { new wxBitmap(filename, wxBITMAP_TYPE_PNG) }
{
    std::cout << "ChatBot Constructor" << std::endl;
}

ChatBot::~ChatBot()
{
    if(_image != NULL) // Attention: wxWidgets used NULL and not nullptr
    {
        delete _image;
        _image = NULL;
    }

    std::cout << "ChatBot Destructor" << std::endl;
}


// Task 2 - RULE OF FIVE 
ChatBot::ChatBot(const ChatBot& source)
{
    _image = new wxBitmap();
    *_image =  *source._image;
    _currentNode = source._currentNode;
    _rootNode = source._rootNode;
    _chatLogic = source._chatLogic;
    _chatLogic->SetChatbotHandle(this);

    std::cout << "ChatBot Copy Constructor" << std::endl;
}

ChatBot& ChatBot::operator=(const ChatBot& source)
{
    if(this == &source)
        return *this;

    delete _image;
    
     _image = new wxBitmap();
     *_image = *source._image;
     _currentNode = source._currentNode;
     _rootNode = source._rootNode; 
     _chatLogic = source._chatLogic;
     _chatLogic->SetChatbotHandle(this);
    
    std::cout << "ChatBot Copy Assignment Operator" << std::endl;

    return *this;
}

ChatBot::ChatBot(ChatBot&& source)
{
    _image = source._image;
    _currentNode = source._currentNode;
    _rootNode =  source._rootNode;
    _chatLogic = source._chatLogic;
    _chatLogic->SetChatbotHandle(this);

    source._image = nullptr;
    source._currentNode = nullptr;
    source._rootNode = nullptr;
    source._chatLogic = nullptr;
    
    std::cout << "ChatBot Move Constructor" << std::endl;  
}

ChatBot& ChatBot::operator=(ChatBot&& source)
{
    if (this == &source)
        return *this;

    delete _image;

    _image = source._image;
    _currentNode = source._currentNode;
    _rootNode =  source._rootNode;
    _chatLogic = source._chatLogic;
    _chatLogic->SetChatbotHandle(this);

    source._image = nullptr;
    source._currentNode = nullptr;
    source._rootNode = nullptr;
    source._chatLogic = nullptr;
    
    std::cout << "ChatBot Move Assignment Operator " << std::endl; 
}
// EOF Task 2 - RULE OF FIVE 

void ChatBot::ReceiveMessageFromUser(std::string message)
{
    typedef std::pair<GraphEdge *, int> EdgeDist;
    std::vector<EdgeDist> levDists; // format is <ptr,levDist>

    for (size_t i = 0; i < _currentNode->GetNumberOfChildEdges(); ++i)
    {
        GraphEdge *edge = _currentNode->GetChildEdgeAtIndex(i);
        for (auto keyword : edge->GetKeywords())
        {
            EdgeDist ed{edge, ComputeLevenshteinDistance(keyword, message)};
            levDists.push_back(ed);
        }
    }

    GraphNode *newNode;
    if (levDists.size() > 0)
    {
        std::sort(levDists.begin(), levDists.end(), [](const EdgeDist &a, const EdgeDist &b) { return a.second < b.second; });
        newNode = levDists.at(0).first->GetChildNode(); 
    }
    else
    {
        newNode = _rootNode;
    }

    _currentNode->MoveChatbotToNewNode(newNode);
}

void ChatBot::SetCurrentNode(GraphNode *node)
{
    _currentNode = node;

    std::vector<std::string> answers = _currentNode->GetAnswers();
    std::mt19937 generator(int(std::time(0)));
    std::uniform_int_distribution<int> dis(0, answers.size() - 1);
    std::string answer = answers.at(dis(generator));

    _chatLogic->SendMessageToUser(answer);
}

int ChatBot::ComputeLevenshteinDistance(std::string s1, std::string s2)
{
    std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
    std::transform(s2.begin(), s2.end(), s2.begin(), ::toupper);

    const size_t m(s1.size());
    const size_t n(s2.size());

    if (m == 0)
        return n;
    if (n == 0)
        return m;

    size_t *costs = new size_t[n + 1];

    for (size_t k = 0; k <= n; k++)
        costs[k] = k;

    size_t i = 0;
    for (std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i)
    {
        costs[0] = i + 1;
        size_t corner = i;

        size_t j = 0;
        for (std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j)
        {
            size_t upper = costs[j + 1];
            if (*it1 == *it2)
            {
                costs[j + 1] = corner;
            }
            else
            {
                size_t t(upper < corner ? upper : corner);
                costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
            }

            corner = upper;
        }
    }

    int result = costs[n];
    delete[] costs;

    return result;
}