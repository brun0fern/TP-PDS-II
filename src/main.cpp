#include <iostream>
#include <vector>
#include <memory>
#include "User.hpp"
#include "Tweet.hpp"
#include "Timeline.hpp"
#include "Retweet.hpp"
#include "NotificationManager.hpp"
#include "MentionNotification.hpp"
#include "NewFollowerNotification.hpp"

// --- Funções Auxiliares ---
void printMainMenu(NotificationManager& nm, User* currentUser) {
    int unread = nm.getUnreadNotifications(currentUser).size();
    std::cout << "\n=== TWITTER CLI ===" << std::endl;
    std::cout << "1. Postar tweet" << std::endl;
    std::cout << "2. Ver timeline" << std::endl;
    std::cout << "3. Retweetar" << std::endl;
    std::cout << "4. Seguir usuário" << std::endl;
    std::cout << "5. Notificações (" << unread << ")" << std::endl;
    std::cout << "6. Sair" << std::endl;
    std::cout << "Escolha: ";
}

void showNotificationsMenu(NotificationManager& nm, User* currentUser) {
    auto unread = nm.getUnreadNotifications(currentUser);
    
    if (unread.empty()) {
        std::cout << "\nNenhuma notificação nova.\n";
        return;
    }

    std::cout << "\n=== NOTIFICAÇÕES ===" << std::endl;
    for (size_t i = 0; i < unread.size(); ++i) {
        std::cout << i+1 << ". [" << unread[i]->getTimestamp() << "] " 
                  << unread[i]->getMessage() << "\n";
    }

    std::cout << "\nDigite o número para marcar como lida (0 para voltar): ";
    int choice;
    std::cin >> choice;
    
    if (choice > 0 && choice <= static_cast<int>(unread.size())) {
        unread[choice-1]->markAsRead();
    }
}

// --- Programa Principal ---
int main() {
    // Inicialização do sistema
    Timeline timeline;
    NotificationManager notificationManager;
    std::vector<std::unique_ptr<User>> users;
    
    // Dados iniciais (para teste)
    users.push_back(std::make_unique<User>("Alice"));
    users.push_back(std::make_unique<User>("Bob"));
    User* currentUser = users[0].get(); // Alice logada

    // Menu interativo
    int choice;
    do {
        printMainMenu(notificationManager, currentUser);
        std::cin >> choice;
        std::cin.ignore(); // Limpar buffer

        switch(choice) {
            case 1: { // Postar tweet
                std::cout << "Digite seu tweet: ";
                std::string content;
                std::getline(std::cin, content);
                
                auto tweet = currentUser->postTweet(content);
                timeline.addTweet(tweet);

                // Disparar notificações de menção
                size_t atPos = content.find('@');
                while (atPos != std::string::npos) {
                    size_t endPos = content.find_first_of(" .,!?", atPos);
                    std::string username = content.substr(atPos+1, endPos - (atPos+1));
                    
                    for (auto& user : users) {
                        if (user->getUsername() == username) {
                            notificationManager.addNotification(
                                new MentionNotification(user.get(), currentUser, tweet)
                            );
                            break;
                        }
                    }
                    atPos = content.find('@', endPos);
                }
                
                std::cout << "Tweet postado!" << std::endl;
                break;
            }
            
            case 2: // Ver timeline
                std::cout << "\n=== TIMELINE ===" << std::endl;
                timeline.print();
                break;
                
            case 3: { // Retweetar
                std::cout << "ID do tweet para retweetar: ";
                int id;
                std::cin >> id;
                
                std::cout << "Adicionar comentário (opcional): ";
                std::string comment;
                std::cin.ignore();
                std::getline(std::cin, comment);
                
                if (Tweet* original = timeline.findTweet(id)) {
                    auto retweet = currentUser->postRetweet(original, comment);
                    timeline.addTweet(retweet);
                    
                    // Notificar autor original
                    if (original->getAuthor() != currentUser) {
                        notificationManager.addNotification(
                            new RetweetNotification(original->getAuthor(), currentUser, retweet)
                        );
                    }
                    std::cout << "Retweet realizado!" << std::endl;
                } else {
                    std::cout << "Tweet não encontrado!" << std::endl;
                }
                break;
            }
                
            case 4: { // Seguir usuário
                std::cout << "Usuário para seguir: ";
                std::string username;
                std::getline(std::cin, username);
                
                for (auto& user : users) {
                    if (user->getUsername() == username) {
                        currentUser->follow(user.get());
                        
                        // Notificar usuário seguido
                        notificationManager.addNotification(
                            new NewFollowerNotification(user.get(), currentUser)
                        );
                        
                        std::cout << "Agora você segue @" << username << "!" << std::endl;
                        break;
                    }
                }
                break;
            }
                
            case 5: // Notificações
                showNotificationsMenu(notificationManager, currentUser);
                break;
        }
    } while (choice != 6);

    // Salvar notificações ao sair (opcional)
    notificationManager.saveToFile("notifications.dat");
    std::cout << "Saindo... Até logo!" << std::endl;
    return 0;
}
