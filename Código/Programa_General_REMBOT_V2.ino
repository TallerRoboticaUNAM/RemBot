//     PROGRAMA GENERAL REMBOT
//     TALLER DE ROBOTICA ABIERTA UNAM


///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////LIBRERIAS//////////////////////////////////////////

#include <RoboClaw.h>                 //LIBRERIA PARA EL CONTROL DEL MOVIMIENTO DE YAW
#include <WiFi.h>                     //LIBRERIA PARA WIFI


//////////////////CREDENCIALES PARA LA COMUNICACION POR WIFI///////////////////////////

//CREDENCIALES VARIABLES, CAMBIAR EN CASO DE SER NECESARIO
String header;
const char* ssid = "";
const char* password = "";
WiFiServer server(80);


/////////////////////////////////ROBOCLAW//////////////////////////////////////////////
RoboClaw roboclaw(&Serial2, 10000);



//////////////////////////////VARIABLES GLOBALES///////////////////////////////////////

///////////////////VELOCIDADES PARA EL CONTROL DE CYTRON //////////////////////////////
//CONSIDERANDO CW--->VEL>127
//CONSIDERANDO CCW--->VEL<128
int VEL_CW = 190; //50%
int VEL_CCW = 63; //50%
int VEL_STOP = 127; //SE DETIENE LOS MOTORES

//////////////////////////////////VARIABLES PARA WIFI//////////////////////////////////

/////////////VARIABLES PARA VERIFICAR EL TIEMPO DE ESPERA DE CONEXION////////////////// 
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////HTML Y CSS/////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

String pagina=
"<!DOCTYPE html>"
"<html lang = 'es'>"

"<head>"
  "<meta charset='utf-8'>"
    "<title> ReMBot</title>"
    "<!-----------------------------CSS---------------------------->"
    "<style>"
    "#titleProy {"
        "color: #000000;"
        "font-family: 'Helvetica';"
        "font-weight: 400;"
        "font-size: 30px;"
        "text-align: center;"
    "}   "

    "#subtitle {"
        "color: #000000;"
        "font-family: 'Helvetica';"
        "font-weight: 400;"
        "font-size: 26px;"
    "}"

    ".subSys{"
        "color: #363638;"
        "margin-left: 4.1cm;"
        "margin-right: 4.1cm;"
        "float: left;"
    "}"

    ".sys{"
        "margin-top: 1.5cm;"
        "margin-left: 5cm;"
        "margin-right: 5cm;"
        "float: left;"
    "}"

    "/*---------------------------------------------------------------*/"

    "/*Locomoción y Yaw*/"

    "#locoYaw .loco{"
  "width: 85px;"
  "height: 25px;"
  "padding: 5px 5px 5px 5px;"
  "background: #5cb85c;"
  "color: #ffffff;"
  "border-color: #4cae4c;"
  "border-width: 1px;"
  "border-style: solid;"
  "border-radius: 3px 3px 3px 3px;"
  "font-family: 'Helvetica';"
  "font-weight: 400;"
  "font-size: 14px;"
  "text-align: center;"
    "}"

    "#locoYaw .stopLY{"
  "width: 85px;"
  "height: 25px;"
  "padding: 5px 5px 5px 5px;"
  "background: #d9534f;"
  "color: #ffffff;"
  "border-color: #d43f3a;"
  "border-width: 1px;"
  "border-style: solid;"
  "border-radius: 3px 3px 3px 3px;"
  "font-family: 'Helvetica';"
  "font-weight: 400;"
  "font-size: 14px;"
  "text-align: center;"
    "}"

    "/*Estilo botones yaw delanteros*/"
    "#Cont .yaw{ "
  "width: 85px;"
  "height: 25px;"
  "padding: 5px 5px 5px 5px;"
  "background: #f0ad4e;"
  "color: #ffffff;"
  "border-color: #eea236;"
  "border-width: 1px;"
  "border-style: solid;"
  "border-radius: 3px 3px 3px 3px;"
  "font-family: 'Helvetica';"
  "font-weight: 400;"
  "font-size: 14px;"
    "text-align: center;"
    "}"

    "/*Estilo botones yaw traseros*/"
    "#Cont .yawT{ "
  "width: 85px;"
  "height: 25px;"
  "padding: 5px 5px 5px 5px;"
  "background: #d4a869;"
  "color: #ffffff;"
  "border-color: #dda557;"
  "border-width: 1px;"
  "border-style: solid;"
  "border-radius: 3px 3px 3px 3px;"
  "font-family: 'Helvetica';"
  "font-weight: 400;"
  "font-size: 14px;"
    "text-align: center;"
    "}"


    "#yaw1LD{"
        "margin-top: 4.4cm;"
        "margin-right: 0.5cm;"
        "margin-left: 2.5cm;"
        "position: absolute;"
    "}"

    "#yaw2RD{"
        "margin-top: 2.5cm;"
        "margin-left: 2.5cm;"
        "position: absolute;"
    "}"

    "#yaw1LT{"
        "margin-top: 5.5cm;"
        "margin-right: 0.5cm;"
        "margin-left: 2.5cm;"
        "position: absolute;"
    "}"

    "#yaw2RT{"
        "margin-top: 3.6cm;"
        "margin-left: 2.5cm;"
        "position: absolute;"
    "}"

    "/*---------------------------------------------------------------*/"

    "/*Roll*/"

    "#Cont2 .rollD{"
  "width: 85px;"
  "height: 25px;"
  "padding: 5px 5px 5px 5px;"
  "background: #5bc0de;"
  "color: #ffffff;"
  "border-color: #46b8da;"
  "border-width: 1px;"
  "border-style: solid;"
  "border-radius: 3px 3px 3px 3px;"
  "font-family: 'Helvetica';"
  "font-weight: 400;"
  "font-size: 14px;"
  "text-align: center;"
    "}"

    "#Cont2 .rollT{"
  "width: 85px;"
  "height: 25px;"
  "padding: 5px 5px 5px 5px;"
  "background: #428bca;"
  "color: #ffffff;"
  "border-color: #357ebd;"
  "border-width: 1px;"
  "border-style: solid;"
  "border-radius: 3px 3px 3px 3px;"
  "font-family: 'Helvetica';"
  "font-weight: 400;"
  "font-size: 14px;"
  "text-align: center;"
   " }"

    "#roll .stopR{"
        "width: 85px;"
        "height: 25px;"
        "padding: 5px 5px 5px 5px;"
        "background: #d9534f;"
        "color: #ffffff;"
        "border-color: #d43f3a;"
        "border-width: 1px;"
        "border-style: solid;"
        "border-radius: 3px 3px 3px 3px;"
        "font-family: 'Helvetica';"
        "font-weight: 400;"
        "font-size: 14px;"
        "text-align: center;"
    "}"

    "#rollDR{"
        "margin-top: 0.2cm;"
        "margin-right: 1cm;"
        "margin-left: 2.5cm;"
        "position: absolute;"
    "}    "
   
    "#rollDL{"
        "margin-top: 2.5cm;"
        "margin-left: 14.9cm;"
        "margin-right: 20cm;"
        "position: absolute;"
    "}    "


    "#rollTR{"
        "margin-top: 1.3cm;"
        "margin-right: 1cm;"
        "margin-left: 2.5cm;"
        "position: absolute;"
    "}    "
   
    "#rollTL{"
        "margin-top: 4cm;"
        "margin-left: 14.9cm;"
        "margin-right: 20cm;"
        "position: absolute;"
    "}    "


    ".stopR{"
        "margin-top: 1cm;"
        "position: absolute;"

    "}"

    "/*---------------------------------------------------------------*/"

    "/*Balanceo*/"

    "#Cont3 .x{"
        "width: 85px;"
        "height: 25px;"
        "padding: 5px 5px 5px 5px;"
        "background: #8c8525;"
        "color: #ffffff;"
        "border-color: #656119;"
        "border-width: 1px;"
        "border-style: solid;"
        "border-radius: 3px 3px 3px 3px;"
        "font-family: 'Helvetica';"
        "font-weight: 400;"
        "font-size: 14px;"
        "text-align: center;"
    "}"

    "#Cont3 .y{"
        "width: 85px;"
        "height: 25px;"
        "padding: 5px 5px 5px 5px;"
        "background: #8c8525;"
        "color: #ffffff;"
        "border-color: #656119;"
        "border-width: 1px;"
        "border-style: solid;"
        "border-radius: 3px 3px 3px 3px;"
        "font-family: 'Helvetica';"
        "font-weight: 400;"
        "font-size: 14px;"
        "text-align: center;"
    "}"

    "#Cont3 .stopB{"
        "width: 85px;"
        "height: 25px;"
        "padding: 5px 5px 5px 5px;"
        "background: #d9534f;"
        "color: #ffffff;"
        "border-color: #d43f3a;"
        "border-width: 1px;"
        "border-style: solid;"
        "border-radius: 3px 3px 3px 3px;"
        "font-family: 'Helvetica';"
        "font-weight: 400;"
        "font-size: 14px;"
        "text-align: center;"
    "}"

    "#yL{"
        "margin-top: 3.5cm;"
        "margin-right: 0.5cm;"
        "margin-left: 24.7cm;"
        "position: absolute;"
    "}"

    "#yR{"
        "margin-top: 1.2cm;"
        "margin-left: 2.5cm;"
        "position: absolute;"
    "}"



    "/*---------------------------------------------------------------*/"

    "/*Paro de emergencia*/"
    "#last{"
        "margin-top: 10cm;"
        "margin-left: 18cm;"
        "text-align: center;"
        "position: absolute;"
    "}"

    "#emergency{"
        "width: 129px;"
        "height: 45px;"
        "padding: 5px 5px 5px 5px;"
        "background: #b71c1c;"
        "color: #ffffff;"
        "border-color: #8a0602;"
        "border-width: 1px;"
        "border-style: solid;"
        "border-radius: 3px 3px 3px 3px;"
        "font-family: 'Helvetica';"
        "font-weight: 400;"
        "font-size: 14px;"
        "text-align: center;"
    "}"

    "/*---------------------------------------------------------------*/"

    "/*Footer*/"
    "footer{"
  "margin-top: 10px;"
  "padding: 10px 10px 10px 10px;"
  "text-align: center;"
    "color: #3f3d3d;"
  "font-size: 16px;"
  "font-weight: bold;"
  "border-radius: 2px;"
    "font-family: 'Nunito', sans-serif;"
    "margin-top: 40%;"
    "}"

    "#piePagina a{"
  "color: #3f3c3c;"
    "}"
    "</style>"
"</head>"


"<!------------------------Body-------------------------->"

"<body>"

    "<div id = 'titleProy'>"
        "<h1>ReMBot</h1> "
    "</div>"

    "<div id='subtitle'>"
        "<div class = 'subSys'>Locomoción/Yaw</div>"
        "<div class = 'subSys' >Roll</div>"
        "<div class = 'subSys' >Balanceo</div>"
    "</div>"

    "<!------------------------Locomoción-------------------------->"
    "<div id = 'Cont'>"
        "<div class = 'sys' id='locoYaw'>"
            "<!--Boton yaw de la derecha delantero-->"
            "<div id = 'yaw2RD'>"
                "<a href='/YawD_F'><button  class = 'yaw'>Right</button></a>"
           "</div>"
           "<!--Boton yaw de la derecha trasero-->"
           "<div id = 'yaw2RT'>"
            "<a href='/YawT_F'><button  class = 'yawT'>Right</button></a>"
            "</div>"
           "<p><a href='/Loc_F'><button class = 'loco' >Forward</button></a></p>"
           "<p><a href='Loc_S'><button class = 'stopLY'>Stop L</button></a></p>"
           "<p><a href='/YawD_S'><button class = 'stopLY'>Stop YD</button></a></p>"
           "<p><a href='/YawT_S'><button class = 'stopLY'>Stop YT</button></a></p>"
           "<p><a href='/Loc_B'><button class = 'loco'>Backward</button></a></p>"
        "</div>"

        "<!--Boton yaw de la izquierda delantero-->"
        "<div id = 'yaw1LD'>"
            "<p><a href='/YawD_B'><button class ='yaw' >Left</button></a></p>"
        "</div>"
         "<!--Boton yaw de la izquierda trasero-->"
        "<div id = 'yaw1LT'>"
            "<p><a href='/YawT_B'><button class ='yawT' >Left</button></a></p>"
        "</div>"
    "</div>"

    "<!------------------------Roll-------------------------->"
   "<div id = 'Cont2'>"
        "<div class = 'sys' id = 'roll'>"
            "<div id = 'rollDR'>"
                "<p><a href='/RollD_F'><button class = 'rollD' >Right</button></a></p>"
            "</div>"
            "<p><a href='/ROLL_S'> <button class = 'stopR'>Stop R</button></a></p>"
            "<div id = 'rollTR'>"
                "<p><a href='/RollT_F'><button class = 'rollT' >Right</button></a></p>"
            "</div>"
        "</div>"
        "<div id = 'rollDL'>"
            "<p><a href='/RollD_B'><button class = 'rollD'>Left</button></a></p>"
        "</div>"

        "<div id = 'rollTL'>"
            "<p><a href='/RollT_B'><button class = 'rollT'>Left</button></a></p>"
        "</div>"

        "<div id = 'last'>"
            "<p><a href='/E_ST'><button id = 'emergency'>Emergency <br> Stop</button></a></p>"
        "</div>"
   "</div>"

    "<!------------------------Balanceo-------------------------->"

    "<div id = 'Cont3'>"
        "<div class = 'sys' >"
            "<div id = 'yR'>"
                "<p><a href='/BM_R'><button class = 'y'>Right</button></a></p>"
            "</div>"
            "<p><a href='/BM_U'><button class = 'x'>Forward</button></a></p>"
            "<p><a href='/BM_S'><button class = 'stopB' >Stop XY</button></a></p>"
            "<p><a href='/BM_D'><button class = 'x'>Backward</button></a></p>"
        "</div>"
        "<div id = 'yL'>"
            "<p><a href='/BM_L'><button class = 'y'>Left</button></a></p>"
        "</div>"
    "</div>"

    "<!------------------------Pie_pag-------------------------->"
    "<footer id='piePagina'>"
        "<p>&copy; 2021 <a href='#'> Talller de Robótica Abierta UNAM</a></p>"
    "</footer>"

"</body>"
"</html>" ; //FIN DE LA PAGINA

///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////PINES A UTILIZAR EN ESP32/////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------LOCOMOCION------------------------------------
//NOTA: POR EL MOMENTO SE MANTENDRA ASI, HASTA NUEVO AVISO 

//PIN (PUEDEN CAMBIAR)
int ML_1 = 35;
int ML_2 = 34;
int ML_3 = 33;
int ML_4 = 32;

//CANALES PARA LOCOMOCION (EN PRINCIPIO NO SE MUEVEN)
int CML_1 = 1;
int CML_2 = 2;
int CML_3 = 3;
int CML_4 = 4;

//---------------------------------------YAW------------------------------------------

// PARA LA COMUNICACION CON LOS MOTORES DE YAW SE HHARA POR MEDIO DEL CONTROLADOR
// ROBOCLAW, UTILIZANDO COMUNICACION SERIAL, YA ANTES DEIFINIDO.

//---------------------------------------ROLL-----------------------------------------

//PIN (PUEDEN CAMBIAR)
//ROLL DELANTERO
int MRO_1 = 27;
//ROLL TRASERO
int MRO_2 = 26;

//CANALES PARA ROLL (EN PRINCIPIO NO SE MUEVEN)
//ROLL DELANTERO
int CMRO_1 = 7;
//ROLL TRASERO
int CMRO_2 = 8;

//---------------------------------------BALANCEO-------------------------------------

//PIN (PUEDEN CAMBIAR)
//EJE X
int MBX = 25;
//EJE Y
int MBY = 23;

//CANALES PARA BALANCEO (EN PRINCIPIO NO SE MUEVEN)
//ROLL DELANTERO
int CMBX = 9;
//ROLL TRASERO
int CMBY = 10;



//////////////////////////////CONFIGURACION////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:

  /////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////COMUNICACION SERIAL//////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////
  
  roboclaw.begin(38400);                    //INICIO DE LA COMUNICACION SERIAL CON LA ROBOCLAW
  Serial.begin(115200);                     //INICIO DE LA COMUNICACION SERIAL

  /////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////CONGIRUACION PARA MOTORES/////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////
  
  /////////////DEFINCION DE CANALES PARA LOS DIFERENTES SISTEMAS///////////////////////
  
  //---------------------------------------LOCOMOCION----------------------------------
  ledcAttachPin(ML_1,CML_1);
  ledcAttachPin(ML_2,CML_2);
  ledcAttachPin(ML_3,CML_3);
  ledcAttachPin(ML_4,CML_4);

  //---------------------------------------YAW----------------------------------------
  //////PARA YAW NO SE DEFINE, DEBIDO A QUE SE UTILIZAR OTRO CONTROLADOR//////////////

  //---------------------------------------ROLL-------------------------------------
  //ROLL DELANTERO
  ledcAttachPin(MRO_1,CMRO_1);
  //ROLL TRASERO
  ledcAttachPin(MRO_2,CMRO_2);

  //---------------------------------------BALANCEO-------------------------------------
  //EJE X
  ledcAttachPin(MBX,CMBX);
  //EJE Y
  ledcAttachPin(MBY,CMBY);

  //DEFINCION DE PARAMETROS PARA LOS CANALES (MISMOS PARAMETROS PARA TODOS LOS CANALES)
  
  //---------------------------------------LOCOMOCION-----------------------------------
  ledcSetup(CML_1,6000,8);
  ledcSetup(CML_2,6000,8);
  ledcSetup(CML_3,6000,8);
  ledcSetup(CML_4,6000,8);

  //---------------------------------------YAW-------------------------------------
  ////////PARA YAW NO SE DEFINE, DEBIDO A QUE SE UTILIZAR OTRO CONTROLADOR//////////////
  
  //---------------------------------------ROLL-----------------------------------------
  //ROLL DELANTERO
  ledcSetup(CMRO_1,6000,8);
  //ROLL TRASERO
  ledcSetup(CMRO_2,6000,8);
  //---------------------------------------BALANCEO-------------------------------------
  //EJE X
  ledcSetup(CMBX,6000,8);
  //EJE Y
  ledcSetup(CMBY,6000,8);

  /////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////CONGIRUACION PARA WIFI////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////  

  //SE CONECTA AL WIFI ESPECIFICADO
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  //SE ESPERA HASTA QUE CONECTE
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //SE IMPRIME LA DIRECCION DE LA PAGINA WEB
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}



//SE DESARROLLA LA PAGINA EN LA FUNCION LOOP
void loop(){
  
  WiFiClient client = server.available();   
  if (client) {                            
    currentTime = millis();
    previousTime = currentTime;
    
    Serial.println("New Client.");          
    String currentLine = "";               
    
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  
      currentTime = millis();
      if (client.available()) {           
        char c = client.read();            
        Serial.write(c);                   
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            /////////////////////////////////////////////////////////////////////////////////////////////
            ////////////////////////FUNCIONES A HACER DEPENDIENDO DEL BOTON APRETADO/////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////////

            /////////////////////////////////////MOVIMIENTO DE YAW///////////////////////////////////////
            
            //////////////////////////////////////YAW DELANTERO//////////////////////////////////////////
            if (header.indexOf("GET /YawD_F") >= 0)
            {
              YAWD_F();
            }
            else if(header.indexOf("GET /YawD_B") >= 0)
            {
              YAWD_B();
            }
            else if(header.indexOf("GET /YawD_S") >= 0)
            {
              YAWD_S();
            }            
            /////////////////////////////////////////////////////////////////////////////////////////////

            //////////////////////////////////////YAW TRASERO//////////////////////////////////////////
            if (header.indexOf("GET /YawT_F") >= 0)
            {
              YAWT_F();
            }
            else if(header.indexOf("GET /YawT_B") >= 0)
            {
              YAWT_B();
            }
            else if(header.indexOf("GET /YawT_S") >= 0)
            {
              YAWT_S();
            }            
            /////////////////////////////////////////////////////////////////////////////////////////////

            /////////////////////////////////////MOVIMIENTO DE ROLL///////////////////////////////////////

            //////////////////////////////////////ROLL DELANTERO//////////////////////////////////////////

            if (header.indexOf("GET /RollD_F") >= 0)
            {
              ROLLD_F();
            }
            else if(header.indexOf("GET /RollD_B") >= 0)
            {
              ROLLD_B();
            }       
            /////////////////////////////////////////////////////////////////////////////////////////////

            //////////////////////////////////////ROLL TRASERO//////////////////////////////////////////

            if (header.indexOf("GET /RollT_F") >= 0)
            {
              ROLLT_F();
            }
            else if(header.indexOf("GET /RollT_B") >= 0)
            {
              ROLLT_B();
            }       
            /////////////////////////////////////////////////////////////////////////////////////////////

            //////////////////////////////////////ROLL TOTAL//////////////////////////////////////////

            if (header.indexOf("GET /ROLL_S") >= 0)
            {
              ROLL_S();
            }
            /////////////////////////////////////////////////////////////////////////////////////////////

            /////////////////////////////////////BALANCEO DE MASA////////////////////////////////////////

            if (header.indexOf("GET /BM_U") >= 0)
            {
              BM_U();
            }
            if (header.indexOf("GET /BM_D") >= 0)
            {
              BM_D();
            }                        
            if (header.indexOf("GET /BM_R") >= 0)
            {
              BM_R();
            }
            if (header.indexOf("GET /BM_L") >= 0)
            {
              BM_L();
            }
            if (header.indexOf("GET /BM_S") >= 0)
            {
              BM_S();
            }            
                                   
            /////////////////////////////////////PARO DE EMERGENCIA////////////////////////////////////////

            if (header.indexOf("GET /E_ST") >= 0)
            {
              E_ST();
            }     
            
            /////////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////CODIGO PARA PAGINA WEB////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////////////////////
            
            client.println(pagina);

            /////////////////////////////////////////////////////////////////////////////////////////////
            
            client.println();
            break;
            
          } else { 
            currentLine = "";
          }
          
        } else if (c != '\r') {  
          currentLine += c;      
        }
        
      }
    }

    
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////FUNCIONES DE MOVIMIENTO////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////MOVIMIENTO DE YAW///////////////////////////////////////////////////////

/////////////////////////////////////////////////////////YAW DELANTERO/////////////////////////////////////////////////////////

void YAWD_F()   //YAW DELANTERO GIRA HACIA ADELANTE
{
    roboclaw.ForwardM1(0x80, 64);
    Serial.println("YAW delantero gira hacia adelante");
}

void YAWD_B()   //YAW DELANTERO GIRA HACIA ATRAS
{
    roboclaw.BackwardM1(0x80, 64);
    Serial.println("YAW delantero gira hacia atras");
}

void YAWD_S()   //YAW DELANTERO GIRA HACIA ATRAS
{
    roboclaw.BackwardM1(0x80, 0);
    Serial.println("YAW delantero se detiene");
}

/////////////////////////////////////////////////////////YAW TRASERO/////////////////////////////////////////////////////////

void YAWT_F()   //YAW TRASERO GIRA HACIA ADELANTE
{
    roboclaw.ForwardM2(0x80, 64);
    Serial.println("YAW trasero gira hacia adelante");
}

void YAWT_B()   //YAW TRASERO GIRA HACIA ATRAS
{
    roboclaw.BackwardM2(0x80, 64);
    Serial.println("YAW trasero gira hacia atras");
}

void YAWT_S()   //YAW TRASERO GIRA HACIA ATRAS
{
    roboclaw.BackwardM2(0x80, 0);
    Serial.println("YAW trasero se detiene");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////MOVIMIENTO DE ROLL///////////////////////////////////////////////////////

/////////////////////////////////////////////////////////ROLL DELANTERO/////////////////////////////////////////////////////////

void ROLLD_F()   //ROLL DELANTERO GIRA HACIA ADELANTE
{
    ledcWrite(CMRO_1,VEL_CCW);
    Serial.println("ROLL delantero gira hacia adelante");
}

void ROLLD_B()   //ROLL DELANTERO GIRA HACIA ATRAS
{
    ledcWrite(CMRO_1,VEL_CW);
    Serial.println("ROLL delantero gira hacia atras");
}

/////////////////////////////////////////////////////////ROLL TRASERO/////////////////////////////////////////////////////////

void ROLLT_F()   //ROLL TRASERO GIRA HACIA ADELANTE
{
    ledcWrite(CMRO_2,VEL_CCW);
    Serial.println("ROLL trasero gira hacia adelante");
}

void ROLLT_B()   //ROLL TRASERO GIRA HACIA ATRAS
{
    ledcWrite(CMRO_2,VEL_CW);
    Serial.println("ROLL trasero gira hacia atras");
}

/////////////////////////////////////////////////////////ROLL TOTAL/////////////////////////////////////////////////////////

void ROLL_S()   //ROLL TOTAL SE DETIENE
{
    ledcWrite(CMRO_1,VEL_STOP);
    ledcWrite(CMRO_2,VEL_STOP);
    Serial.println("ROLL trasero gira hacia atras");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////BALANCEO DE MASA////////////////////////////////////////////////////////

void BM_U()   //BALANCEO DE MASA HACIA ARRIBA
{
    ledcWrite(CMBY,VEL_CW);
    Serial.println("Motor de Y impulsa hacia delante el mecanismo");
}

void BM_D()   //BALANCEO DE MASA HACIA ABAJO
{
    ledcWrite(CMBY,VEL_CCW);
    Serial.println("Motor de Y impulsa hacia atras el mecanismo");
}

void BM_R()   //BALANCEO DE MASA HACIA LA DERECHA
{
    ledcWrite(CMBX,VEL_CW);
    Serial.println("Motor de X impulsa hacia delante el mecanismo");
}

void BM_L()   //BALANCEO DE MASA HACIA LA IZQUIERDA
{
    ledcWrite(CMBX,VEL_CCW);
    Serial.println("Motor de X impulsa hacia atras el mecanismo");
}

void BM_S()   //BALANCEO DE MASA SE DETIENE
{
    ledcWrite(CMBX,VEL_STOP);
    ledcWrite(CMBY,VEL_STOP);
    Serial.println("Motores de balanceo se detienen");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////PARO DE EMERGENCIA//////////////////////////////////////////////////////

void E_ST()   //TODO SE DETIENE
{
    //YAW
    roboclaw.BackwardM1(0x80, 0);
    roboclaw.BackwardM2(0x80, 0);
    //ROLL
    ledcWrite(CMRO_1,VEL_STOP);
    ledcWrite(CMRO_2,VEL_STOP);
    //BALANCEO DE MASA
    ledcWrite(CMBX,VEL_STOP);
    ledcWrite(CMBY,VEL_STOP);
    Serial.println("Todo se detiene");
}
