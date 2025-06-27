#include "MentionNotification.hpp"
#include <sstream>
#include <stdexcept>

// Construtor
MentionNotification::MentionNotification(User* recipient, User* sender, Tweet* tweet)
    : Notification(recipient, sender), mentionedTweet(tweet) {
    
    if (!tweet) {
        throw std::invalid_argument("Tweet mencionado não pode ser nulo");
    }
    
    // Verifica se o recipient foi realmente mencionado
    auto mentions = tweet->getMentions();
    if (mentions.find(recipient->getUsername()) == mentions.end()) {
        throw std::logic_error("O usuário não foi mencionado neste tweet");
    }
}

// Mensagem formatada
std::string MentionNotification::getMessage() const {
    std::string contentPreview = mentionedTweet->getContent();
    if (contentPreview.length() > 30) {
        contentPreview = contentPreview.substr(0, 27) + "...";
    }
    
    return "@" + sender->getUsername() + " mencionou você em: \"" + contentPreview + "\"";
}

// Tipo da notificação
NotificationType MentionNotification::getType() const {
    return NotificationType::MENTION;
}

// Serialização para persistência
std::string MentionNotification::serialize() const {
    std::ostringstream oss;
    oss << "MENTION|"
        << recipient->getUsername() << "|"
        << sender->getUsername() << "|"
        << mentionedTweet->getId() << "|"
        << std::chrono::system_clock::to_time_t(timestamp) << "|"
        << (read ? "1" : "0");
    return oss.str();
}

// Desserialização
std::unique_ptr<MentionNotification> MentionNotification::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string type;
    std::getline(iss, type, '|');
    
    if (type != "MENTION") {
        throw std::runtime_error("Tipo de notificação inválido");
    }
    
    std::string recipientName, senderName;
    int tweetId, timestamp;
    bool isRead;
    
    std::getline(iss, recipientName, '|');
    std::getline(iss, senderName, '|');
    iss >> tweetId;
    iss.ignore(); // Ignora o separador
    iss >> timestamp;
    iss.ignore();
    iss >> isRead;
    
    // Em um sistema real, você precisaria acessar:
    // 1. UserManager para obter os usuários
    // 2. TweetManager para obter o tweet
    // Exemplo simplificado:
    User* recipient = UserManager::getInstance().getUser(recipientName);
    User* sender = UserManager::getInstance().getUser(senderName);
    Tweet* tweet = TweetManager::getInstance().getTweet(tweetId);
    
    auto notification = std::make_unique<MentionNotification>(recipient, sender, tweet);
    notification->timestamp = std::chrono::system_clock::from_time_t(timestamp);
    if (isRead) notification->markAsRead();
    
    return notification;
}
