#include "NewFollowerNotification.hpp"
#include <sstream>
#include <stdexcept>

// Construtor
NewFollowerNotification::NewFollowerNotification(User* recipient, User* newFollower) 
    : Notification(recipient, newFollower) {
    
    if (!newFollower) {
        throw std::invalid_argument("Novo seguidor não pode ser nulo");
    }
}

// Mensagem formatada
std::string NewFollowerNotification::getMessage() const {
    return "@" + sender->getUsername() + " começou a seguir você!";
}

// Tipo de notificação
NotificationType NewFollowerNotification::getType() const {
    return NotificationType::NEW_FOLLOWER;
}

// Serialização
std::string NewFollowerNotification::serialize() const {
    std::ostringstream oss;
    oss << "NEW_FOLLOWER|"
        << recipient->getUsername() << "|"
        << sender->getUsername() << "|"
        << std::chrono::system_clock::to_time_t(timestamp) << "|"
        << (read ? "1" : "0");
    return oss.str();
}

// Desserialização
std::unique_ptr<NewFollowerNotification> NewFollowerNotification::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string type;
    std::getline(iss, type, '|');
    
    if (type != "NEW_FOLLOWER") {
        throw std::runtime_error("Tipo de notificação inválido");
    }
    
    std::string recipientName, senderName;
    time_t timestamp;
    bool isRead;
    
    std::getline(iss, recipientName, '|');
    std::getline(iss, senderName, '|');
    iss >> timestamp;
    iss.ignore();
    iss >> isRead;
    
    User* recipient = UserManager::getInstance().getUser(recipientName);
    User* sender = UserManager::getInstance().getUs
