const char ASSET_DASHBOARD_SERVER_HTML[] PROGMEM = "<!doctypehtml><html lang=\"en\"><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\"content=\"IE=edge\"><meta name=\"viewport\"content=\"width=device-width,initial-scale=1\"><title>BetterTap</title><link rel=\"stylesheet\"href=\"/css/style.css\"><div class=\"container\"><h1>Server</h1><h2>MQTT</h2><form action=\"/cfg/mqtt\"method=\"post\"><table><tr><td>Host:<td><input name=\"host\"value=\"{{HOST}}\"><tr><td>Port:<td><input type=\"number\"name=\"port\"value=\"{{PORT}}\"><tr><td>Client ID:<td><input name=\"clientID\"value=\"{{CLIENT_ID}}\"><tr><td>Username:<td><input name=\"username\"value=\"{{USERNAME}}\"><tr><td>Password:<td><input type=\"password\"name=\"password\"><tr><td>Topic:<td><input name=\"initTopic\">{{INIT_TOPIC}}<tr><td>V1 Topic:<td><input name=\"v1Topic\">{{V1_TOPIC}}</table><button type=\"submit\">Save</button></form></div>";
const unsigned long ASSET_DASHBOARD_SERVER_HTML_LEN = sizeof(ASSET_DASHBOARD_SERVER_HTML);
