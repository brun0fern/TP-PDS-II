#include "Tweet.hpp"
#include "User.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

// --- Tweet Base ---
Tweet::Tweet(int id, User* author, const std::string& content) 
    : id(id), author(author), content(content) {
    if (!author) throw std::invalid_argument("Autor não pode ser nulo");
    if (!isValidContent(content)) throw std::invalid_argument("Conteúdo inválido");
    
    timestamp = std::time(nullptr);
    extractMetadata();
}

void Tweet::extractMetadata() {
    std::istringstream iss(content);
    std::string word;
    
    while (iss >> word) {
        if (word[0] == '#' && word.size() > 1) {
            hashtags.insert(word);
        } 
        else if (word[0] == '@' && word.size() > 1) {
            mentions.insert(word.substr(1)); // Remove '@'
        }
    }
}

void Tweet::display() const {
    std::cout << "\n\033[1;36m@" << author->getUsername() << "\033[0m ["
              << getFormattedTime() << "]\n";
    std::cout << content << "\n";
    std::cout << "\033[1;31m♥ " << likes << " curtidas \033[0m | "
              << "\033[1;32m💬 " << comments.size() << " comentários\033[0m\n";
    
    if (!hashtags.empty()) {
        std::cout << "\n\033[1;34mHashtags:\033[0m ";
        for (const auto& tag : hashtags) {
            std::cout << tag << " ";
        }
    }
}

void Tweet::like() {
    likes++;
}

void Tweet::addComment(const std::string& comment) {
    if (comment.empty()) return;
    comments.push_back(comment);
}

// --- Getters ---
int Tweet::getId() const { return id; }
User* Tweet::getAuthor() const { return author; }
std::string Tweet::getContent() const { return content; }

std::string Tweet::getFormattedTime() const {
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", std::localtime(&timestamp));
    return buffer;
}

// --- Retweet ---
Retweet::Retweet(int id, User* author, Tweet* original, const std::string& comment) 
    : Tweet(id, author, "RT @" + original->getAuthor()->getUsername() + ": " + original->getContent()),
      originalTweet(original), comment(comment) {
    isRetweet = true;
    if (!original) throw std::invalid_argument("Tweet original inválido");
}

void Retweet::display() const {
    std::cout << "\n\033[1;35m↻ @" << author->getUsername() << " retweetou:\033[0m\n";
    originalTweet->display();
    if (!comment.empty()) {
        std::cout << "\n\033[1;33mComentário:\033[0m " << comment << "\n";
    }
}

// --- Validação ---
bool Tweet::isValidContent(const std::string& content) {
    return !content.empty() && content.size() <= 280;
}
