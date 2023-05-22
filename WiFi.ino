void configureAP()
{
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // HTTP handler assignment
  webserver.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html_gz, sizeof(index_html_gz));
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });

  //start server
  webserver.begin();
  server.listen(82);
  Serial.print("Is server live? ");
  Serial.println(server.available());

}

void printTurns()
{
  Serial.print("my target: ");
  Serial.println(spinTarget);
  Serial.print("target count: ");
  Serial.println(targetCount);
  Serial.print("targets done: ");
  Serial.println(targetsDone);
  for (int i = 0; i < 10; i++)
  {
    Serial.print("type: ");
    Serial.print(turns[i].t);
    Serial.print(" -- fields: ");
    for (int f = 0; f < 8; f++)
    {
      if (!turns[i].fields[f]) break;
      Serial.print(turns[i].fields[f]);
      Serial.print(", ");
    }
    Serial.println();
  }
}

void sendState()
{
  printTurns();
  String output = "u";
  output += 'a';
  for (int j = 0; j < 8; j++)
  {
    if (lastturn.fields[j]) output += (char) (lastturn.fields[j] + 48);
    else break;
  }
  for (int i = targetsDone; i < targetCount; i++)
  {
    char head = turns[i].t;
    Serial.print(i);
    Serial.print(turns[i].t);
    if (turns[i].t == 'n') break; //empty
    output += turns[i].t; //add header
    for (int j = 0; j < 8; j++)
    {
      if (turns[i].fields[j]) output += (char) (turns[i].fields[j] + 48);
      else break;
    }
  }
  Serial.print("sending: ");
  Serial.println(output);
  client.send(output);
}
void handle_message(WebsocketsMessage msg) {
  int l = msg.data().length();
  Serial.println(msg.data());
  int count = 0;
  int index = 0;
  char head = msg.data()[0];
  if (head == 'r')
  {
    char digits [3];
    for (int i = 1; i < l; i++)
    {
      if (i > 3) break;
      digits[i - 1] = msg.data()[i];
    }
    int rotation = 0;
    sscanf(digits, "%d", rotation);
    Serial.println(rotation);
  }
  else if (head == 's') sendState();
  else
  {
    clearTurns();
    for (int i = 0; i < l; i++)
    {
      char c = msg.data()[i];
      // if force
      //check end
      if (count == 10) break;
      if (c == 'a' || c == 'q') turns[count].t = c;
      else if (c >= 48 && c <= 57)
      {
        turns[count].fields[index] = (uint8_t) c - 48;
        index++;
      }
      else if (c == ',')
      {
        index = 0;
        count++;
      }
    }
    targetCount = count;
    targetsDone = 0;
    if (turns[0].t == 'a' && !spinning && !hastask) spinup();
  }
}
