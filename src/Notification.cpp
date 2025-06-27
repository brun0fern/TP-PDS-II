#include "Notification.hpp"
#include <iomanip>
#include <sstream>

// --- Notification ---
Notification::Notification(User* recipient, User* sender) 
    : recipient(recipient), sender(sender), 
      timestamp(std::chrono::system_clock::now()) {}

void Notification::markAsRead() { read = true; }
bool Notification::isRead() const { return read; }

std::string Notification::getTimestamp() const {
    auto time = std::chrono::system_clock::to_time_t(timestamp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%d/%m/%Y %H:%M");
    return ss.str();
}

// --- MentionNotification ---
MentionNotification::MentionNotification(User* recipient, User* sender, Tweet* tweet)
    : Notification(recipient, sender), mentionedTweet(tweet) {}

std::string MentionNotification::getMessage() const {
    return "@" + sender->getUsername() + " mencionou você em: \"" 
           + mentionedTweet->getContent().substr(0, 30) + "...\"";
}

NotificationType MentionNotification::getType() const {
    return NotificationType::MENTION;
}

// [Implementar outras classes de notificação aqui]
