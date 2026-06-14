#include <Rcpp.h>

extern "C"
{
#include "client_lib.h"
}

// [[Rcpp::export]]
void peer_connect_r(std::string username, std::string password)
{
    int ret = peer_connect(username.c_str(), password.c_str());
    if (ret < 0)
    {
        Rcpp::stop("Failed to connect to server");
    }
}

// [[Rcpp::export]]
void peer_get_user_r(std::string username, std::string password)
{
    int ret = peer_get_user(username.c_str(), password.c_str());
    if (ret < 0)
    {
        Rcpp::stop("Failed to request peer info");
    }
}

// [[Rcpp::export]]
void peer_send_message_r(std::string recipient, std::string message)
{
    int ret = peer_send_message(recipient.c_str(), message.c_str());
    if (ret < 0)
    {
        Rcpp::stop("Failed to send message to '" + recipient + "'");
    }
}

// [[Rcpp::export]]
void peer_send_ping_r(std::string recipient)
{
    int ret = peer_send_ping(recipient.c_str());
    if (ret < 0)
    {
        Rcpp::stop("Failed to ping '" + recipient + "'");
    }
}

// [[Rcpp::export]]
int peer_receive_r(int timeout_ms = 100)
{
    return peer_receive(timeout_ms);
}

// [[Rcpp::export]]
void peer_disconnect_r()
{
    peer_disconnect();
}

// [[Rcpp::export]]
int peer_get_fd_r()
{
    return peer_get_fd();
}
