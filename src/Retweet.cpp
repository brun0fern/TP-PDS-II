#include "Retweet.hpp"
#include <stdexcept>

// Construtor
Retweet::Retweet(int id, User* author, Tweet* original, const std::string& comment)
    : Tweet(id, author, "RT @" + original->getAuthor()->getUsername() + ": " + original->getContent()),
      originalTweet(original), comment(comment) {
    
    // Validações
    if (!original) {
        throw std::invalid_argument("Tweet original não pode ser nulo");
    }
    
    if (!comment.empty() && !isValidComment(comment)) {
        throw std::invalid_argument("Comentário inválido");
    }
    
    // Marca como retweet
    this->isRetweet = true;
    
    // Herda hashtags e menções do original
    this->hashtags = original->getHashtags();
    this->mentions = original->getMentions();
}

// Exibição formatada
void Retweet::display() const {
    // Cabeçalho do retweet
    std::cout << "\n\033[1;35m↻ @" << author->getUsername();
    if (!comment.empty()) {
        std::cout << " comentou: \"" << comment << "\"";
    }
    std::cout << "\033[0m\n";
    
    // Tweet original
    std::cout << "  \033[1;90mOriginal: \033[0m";
    originalTweet->display();
    
    // Estatísticas do retweet
    std::cout << "  \033[1;33m♻ " << likes << " curtidas neste retweet\033[0m\n";
}

// Getters
Tweet* Retweet::getOriginalTweet() const {
    return originalTweet;
}

std::string Retweet::getComment() const {
    return comment;
}

// Validação
bool Retweet::isValidComment(const std::string& comment) {
    return comment.size() <= 140; // Limite para comentários
}
