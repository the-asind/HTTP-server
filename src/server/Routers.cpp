#include "Routers.hpp"
#include "UserRepository.hpp"
#include "User.hpp"
#include "Utility.hpp"

UserRepository repos;

Role getRole(const std::string& role) {
    if (role == "admin")
        return Role::ADMIN;
    return Role::USER;
}

int getId(const std::string& id_param) {
    int id;
    size_t pos;
    id = std::stoi(id_param, &pos); //str to int
    if (pos != id_param.size()) {
        throw std::invalid_argument(id_param);
    }
    
    return id;
}

void parseAuthData(const std::string& encodedAuthData, std::string &login, std::string &password) {
    std::string auth = encodedAuthData.substr(6, encodedAuthData.length());
    auto data = utils::Split(auth, ":");
    login = utils::DecodeBase64(data[0]);
    password = utils::DecodeBase64(data[1]);
}

void route::RegisterResources(hv::HttpService &router)
{
    router.POST("/user", [](HttpRequest *req, HttpResponse *resp){
        resp->content_type = APPLICATION_JSON;
        nlohmann::json req_body, resp_body;

        try
        {
            req_body = nlohmann::json::parse(req->body);
            auto login = req_body.at("login").get<std::string>();
            auto password = req_body.at("password").get<std::string>();
            User user(login, password, Role::USER);
            resp_body["id"] = repos.createUser(user);
            resp->SetBody(resp_body.dump());
        }
        catch(const std::exception& e)
        {
            resp_body["error"] = "Invalid JSON";
            std::cout << e.what() << std::endl;
            resp->SetBody(resp_body.dump());
            return 400;
        }

        return 201;
    });

    router.PUT("/user/{id}", [](HttpRequest *req, HttpResponse *resp) {
        resp->content_type = APPLICATION_JSON;
        if (req->headers.find("Authorization") == req->headers.end()) {
            resp->headers["WWW-Authenticate"] = "Basic realm=\"Access to the user database\"";
            return 401;
        }

        nlohmann::json resp_body, req_body;
        User user;
        int id;

        try {
            std::string authLogin, authPassword;
            parseAuthData(req->headers["Authorization"], authLogin, authPassword);

            user = repos.getUser(authLogin);

            id = getId(req->GetParam("id"));

            if (!user.validatePassword(authPassword)) {
                return 403;
            }

            if (user.getRole() != Role::ADMIN) {
                if (repos.getUser(id) != user) {
                    return 403;
                }
            }

            req_body = nlohmann::json::parse(req->body);

            std::cout << id << std::endl;

            User storedUser = repos.getUser(id);
            auto login = req_body.at("login").get<std::string>();
            auto password = req_body.at("password").get<std::string>();
            Role userRole;

            if (user.getRole() == Role::ADMIN) {
                userRole = getRole(req_body.at("role").get<std::string>());
            }
            else {
                userRole = Role::USER;
            }

            User putUser(login, password, userRole);
            repos.updateUser(id, putUser);

            resp_body = putUser.toJson();
            resp->SetBody(resp_body.dump());
        }
        catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
            resp_body["error"] = "invalid id type";
            resp->SetBody(resp_body.dump());
            return 400;
        }
        catch (const std::out_of_range& e)
        {
            std::cout << e.what() << std::endl;
            resp_body["error"] = "user id not found";
            resp->SetBody(resp_body.dump());
            return 404;
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
            return 400;
        }

        return 200;
    });

    router.GET("/users", [](HttpRequest *req, HttpResponse *resp) {
        resp->content_type = APPLICATION_JSON;
        if (req->headers.find("Authorization") == req->headers.end()) {
            resp->headers["WWW-Authenticate"] = "Basic realm=\"Access to the user database\"";
            return 401;
        }

        User user;
        try {
            std::string login, password;
            parseAuthData(req->headers["Authorization"], login, password);
            user = repos.getUser(login);

            if (!user.validatePassword(password)) {
                return 403;
            }

            if (user.getRole() != Role::ADMIN) {
                return 403;
            }

            nlohmann::json response;
            for (const std::pair<const int, User>& pair : repos.getUsers()) {
                nlohmann::json jsonUser;
                jsonUser = pair.second.toJson();
                jsonUser["id"] = pair.first;

                response.push_back(jsonUser);
            }

            resp->SetBody(response.dump());
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
            return 400;
        }

        return 200;
    });

    router.GET("/user/{id}", [](HttpRequest *req, HttpResponse *resp){
        resp->content_type = APPLICATION_JSON;
        if (req->headers.find("Authorization") == req->headers.end()) {
            resp->headers["WWW-Authenticate"] = "Basic realm=\"Access to the user database\"";
            return 401;
        }

        nlohmann::json resp_body;
        User user;
        int id;
        try {
            std::string login, password;
            parseAuthData(req->headers["Authorization"], login, password);

            user = repos.getUser(login);
            id = getId(req->GetParam("id"));
            std::cout << id << std::endl;

            if (!user.validatePassword(password)) {
                return 403;
            }

            if (user.getRole() != Role::ADMIN) {
                if (repos.getUser(id) != user) {
                    return 403;
                }
            }

            resp_body = repos.getUser(id).toJson();
            resp_body["id"] = id;
            resp->SetBody(resp_body.dump());
        }
        catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
            resp_body["error"] = "invalid id type";
            resp->SetBody(resp_body.dump());
            return 400;
        }
        catch (const std::out_of_range& e) {
            std::cout << e.what() << std::endl;
            resp->SetBody(resp_body.dump());
            return 404;
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
            return 400;
        }

        return 200;
    });

    router.Delete("/user/{id}", [](HttpRequest *req, HttpResponse *resp){
        resp->content_type = APPLICATION_JSON;
        if (req->headers.find("Authorization") == req->headers.end()) {
            resp->headers["WWW-Authenticate"] = "Basic realm=\"Access to the user database\"";
            return 401;
        }

        nlohmann::json response_body;
        User user;
        int id;
        try {
            std::string login, password;
            parseAuthData(req->headers["Authorization"], login, password);

            user = repos.getUser(login);
            id = getId(req->GetParam("id"));

            if (!user.validatePassword(password)) {
                return 403;
            }

            if (user.getRole() != Role::ADMIN) {
                if (repos.getUser(id) != user) {
                    return 403;
                }
            }

            repos.removeUser(id);
        }
        catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
            response_body["error"] = "invalid id type";
            resp->SetBody(response_body.dump());
            return 400;
        }
        catch (const std::out_of_range& e) {
            std::cout << e.what() << std::endl;
            resp->SetBody(response_body.dump());
            return 404;
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
            return 400;
        }

        return 200;
    });
}