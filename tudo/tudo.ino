#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

// Defina as credenciais do Wi-Fi
const char* ssid = "Pedro";
const char* password = "66996984391";

// Comunicação Serial com o Arduino Mega
SoftwareSerial megaSerial(5, 4); // Pinos D6 (RX) e D5 (TX) do ESP8266

// Inicialize o servidor na porta 80
ESP8266WebServer server(80);

// Função para servir o HTML
const char* htmlPage = R"rawliteral(
  <!DOCTYPE html>
<html lang="pt-BR">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Controle Braçudo</title>
    <style>
        /* Estilos Gerais */
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background-color: #f0f2f5;
            margin: 0;
            color: #333;
        }

        /* Cabeçalho */
        header {
            background-color: #007bff;
            color: #fff;
            padding: 20px 0;
            text-align: center;
        }

        header h1 {
            margin: 0;
            font-size: 2.5em;
        }

        header p {
            margin: 5px 0 0 0;
            font-size: 1.1em;
        }

        /* Rodapé */
        footer {
            background-color: #007bff;
            color: #fff;
            text-align: center;
            padding: 10px 0;
            position: fixed;
            width: 100%;
            bottom: 0;
        }

        /* Contêiner Principal */
        .main-container {
            display: flex;
            flex-wrap: wrap;
            justify-content: center;
            padding: 20px;
            margin-bottom: 60px; /* Espaço para o rodapé */
        }

        /* Estilo dos Motores */
        .container {
            background: #fff;
            border-radius: 8px;
            box-shadow: 0 4px 6px rgba(0,0,0,0.1);
            padding: 20px;
            width: 280px;
            margin: 15px;
            text-align: center;
        }

        .container h2 {
            margin-top: 0;
            color: #007bff;
        }

        .slider-label {
            font-weight: bold;
            margin-top: 20px;
        }

        .slider-values {
            flex-direction: column;
        }
        .slider-values input{
            font-size: 20px;
        }
        .slider-values,
        .slider-controls {
            font-size: 20px;
            margin: 10px 0;
            display: flex;
            align-items: center;
            justify-content:center;
        }

        .slider-values label,
        .slider-controls label {
            width: 48%;
        }

        input[type="number"],
        input[type="range"] {
            width: 100%;
            padding: 6px;
            margin-top: 5px;
            border: 1px solid #ccc;
            border-radius: 4px;
            text-align: center;
        }

        /* Estilo do Slider */
        input[type="range"] {
            -webkit-appearance: none;
            height: 6px;
            border-radius: 5px;
            background: #ddd;
            outline: none;
            margin-top: 15px;
        }

        input[type="range"]::-webkit-slider-thumb {
            -webkit-appearance: none;
            width: 18px;
            height: 18px;
            border-radius: 50%;
            background: #007bff;
            cursor: pointer;
            box-shadow: 0 2px 4px rgba(0,0,0,0.2);
        }

        input[type="range"]::-moz-range-thumb {
            width: 18px;
            height: 18px;
            border-radius: 50%;
            background: #007bff;
            cursor: pointer;
            box-shadow: 0 2px 4px rgba(0,0,0,0.2);
        }

        /* Visualização do Ângulo */
        .angle-visual-container {
            height: 100px;
            width: 100%;
            background-color: #f9f9f9;
            position: relative;
            margin: 20px 0;
            border: 1px solid #ddd;
            border-radius: 4px;
        }

        .angle-visual {
            width: 4px;
            height: 90px;
            background-color: #007bff;
            position: absolute;
            bottom: 5px;
            left: 50%;
            transform-origin: bottom center;
            transition: transform 0.2s ease;
            border-radius: 2px;
        }

        /* Botões de Configuração */
        .config-buttons {
            width: 100%;
            text-align: center;
            margin: 20px 0;
        }

        .config-buttons button,
        .config-buttons select {
            padding: 10px 20px;
            margin: 5px 10px;
            font-size: 16px;
            cursor: pointer;
        }

        .config-buttons button {
            background-color: #007bff;
            color: #fff;
            border: none;
            border-radius: 4px;
        }

        .config-buttons button:hover {
            background-color: #0056b3;
        }

        .config-buttons select {
            padding: 10px;
            font-size: 16px;
            border-radius: 4px;
            border: 1px solid #ccc;
        }

        /* Estilo para transformar o h2 em um botão */
        h2 {
            padding: 10px 20px;
            margin: 10px 0;
            border-radius: 4px;
            cursor: pointer;
            font-size: 1.5em;
            text-align: center;
            transition: background-color 0.3s ease;
        }

        h2:hover {
            background-color: #0056b3;
        }

        h2:active {
            background-color: #003f7f;
        }

        .modal {
            display: none; 
            position: fixed; 
            z-index: 1; 
            left: 0;
            top: 0;
            width: 100%;
            height: 100%;
            overflow: auto; 
            background-color: rgba(0, 0, 0, 0.4); 
        }

        .modal-content {
            background-color: #fefefe;
            margin: 15% auto; 
            padding: 20px;
            border: 1px solid #888;
            width: 80%; 
            max-width: 500px; 
            border-radius: 1em;
            text-align: center;
        }

        .modal button{
            background-color: #007bff;
            border: none;
            color: white;
            padding: 10px 20px;
            margin: 10px 0;
            border-radius: 4px;
            cursor: pointer;
            font-size: 1.5em;
            text-align: center;
            transition: background-color 0.3s ease;
        }

        .close {
            color: #aaa;
            float: right;
            font-size: 28px;
            font-weight: bold;
        }

        .close:hover,
        .close:focus {
            color: black;
            text-decoration: none;
            cursor: pointer;
        }

        /* Responsividade */
        @media (max-width: 768px) {
            .container {
                width: 90%;
            }
        }
    </style>
</head>

<body>

    <header>
        <h1>Braçudo</h1>
        <p>Projeto desenvolvido para a faculdade FASTECH por Giuseph, João, Rafael e Jean</p>
    </header>
    <!-- Contêiner principal -->
    <div class="main-container">
        <!-- Botões de Configuração -->
        <div class="config-buttons">
            <button onclick="saveConfiguration()">Salvar Configuração</button>
            <select id="configurations-dropdown" onchange="loadSelectedConfiguration()">
                <option value="">Selecione uma Configuração</option>
                <!-- As opções serão preenchidas dinamicamente -->
            </select>
            <button onclick="Subir_todos()">Carregar Configuração P/Arduino</button>
        </div>

        <div class="container" id="motor1-container">
            <h2 id="motor1-title" onclick="testMotor('motor1')" onmouseover="changeText(this, 'Testar Motor?')"
                onmouseout="changeText(this, 'Motor X_Y')">
                Motor X_Y
            </h2>
            <div class="slider-values">
                <label>Velocidade: <input type="number" id="motor1-value_vel" value="0" min="200" max="2500" step="1"
                        onclick="updateFromInput_vel('motor1')"></label>
                <label>Angulo Atual: <input type="number" id="motor1-value" value="0" min="-90" max="90" step="1"
                        oninput="updateFromInput('motor1')"></label>
            </div>
            <div class="slider-controls">
                <label>Mínimo: <input type="number" id="motor1-min" value="-90" oninput="updateRange('motor1')"></label>
                <label>Máximo: <input type="number" id="motor1-max" value="90" oninput="updateRange('motor1')"></label>
            </div>
            <div class="angle-visual-container">
                <div class="angle-visual" id="motor1-line"></div>
            </div>
            <label class="slider-label">Controle (graus):</label>
            <input type="range" id="motor1-slider" min="-90" max="90" value="0" oninput="updateValue('motor1')">
        </div>

        <!-- Motor 2 -->
        <div class="container" id="motor2-container">
            <h2 id="motor2-title" onclick="testMotor('motor2')" onmouseover="changeText(this, 'Testar Motor?')"
                onmouseout="changeText(this, 'Motor Duplo')">
                Motor Duplo
            </h2>
            <div class="slider-values">
                <label>Velocidade: <input type="number" id="motor2-value_vel" value="0" min="200" max="2500" step="1"
                        onclick="updateFromInput_vel('motor2')"></label>
                <label>Angulo Atual: <input type="number" id="motor2-value" value="0" min="-90" max="90" step="1"
                        oninput="updateFromInput('motor2')"></label>
            </div>
            <div class="slider-controls">
                <label>Mínimo: <input type="number" id="motor2-min" value="-90" oninput="updateRange('motor2')"></label>
                <label>Máximo: <input type="number" id="motor2-max" value="90" oninput="updateRange('motor2')"></label>
            </div>
            <div class="angle-visual-container">
                <div class="angle-visual" id="motor2-line"></div>
            </div>
            <label class="slider-label">Controle (graus):</label>
            <input type="range" id="motor2-slider" min="-90" max="90" value="0" oninput="updateValue('motor2')">
        </div>

        <!-- Motor 3 -->
        <div class="container" id="motor3-container">
            <h2 id="motor3-title" onclick="testMotor('motor3')" onmouseover="changeText(this, 'Testar Motor?')"
                onmouseout="changeText(this, 'Motor Redutor')">
                Motor Redutor
            </h2>
            <div class="slider-values">
                <label>Velocidade: <input type="number" id="motor3-value_vel" value="0" min="200" max="2500" step="1"
                        onclick="updateFromInput_vel('motor3')"></label>
                <label>Angulo Atual: <input type="number" id="motor3-value" value="0" min="-90" max="90" step="1"
                        oninput="updateFromInput('motor3')"></label>
            </div>
            <div class="slider-controls">
                <label>Mínimo: <input type="number" id="motor3-min" value="-90" oninput="updateRange('motor3')"></label>
                <label>Máximo: <input type="number" id="motor3-max" value="90" oninput="updateRange('motor3')"></label>
            </div>
            <div class="angle-visual-container">
                <div class="angle-visual" id="motor3-line"></div>
            </div>
            <label class="slider-label">Controle (graus):</label>
            <input type="range" id="motor3-slider" min="-90" max="90" value="0" oninput="updateValue('motor3')">
        </div>

        <div class="container" id="motor4-container">
            <h2 id="motor4-title" onclick="testMotor('motor4')" onmouseover="changeText(this, 'Testar Motor?')"
                onmouseout="changeText(this, 'Motor Garra')">
                Motor Garra
            </h2>
            <div class="slider-values">
                <label>Velocidade: <input type="number" id="motor4-value_vel" value="0" min="200" max="2500" step="1"
                        onclick="updateFromInput_vel('motor4')"></label>
                <label>Angulo Atual: <input type="number" id="motor4-value" value="0" min="-90" max="90" step="1"
                        oninput="updateFromInput('motor4')"></label>
            </div>
            <div class="slider-controls">
                <label>Mínimo: <input type="number" id="motor4-min" value="-90" oninput="updateRange('motor4')"></label>
                <label>Máximo: <input type="number" id="motor4-max" value="90" oninput="updateRange('motor4')"></label>
            </div>
            <div class="angle-visual-container">
                <div class="angle-visual" id="motor4-line"></div>
            </div>
            <label class="slider-label">Controle (graus):</label>
            <input type="range" id="motor4-slider" min="-90" max="90" value="0" oninput="updateValue('motor4')">
        </div>
        <!-- Motor 4 -->
        <div class="container" id="motor5-container">
            <h2 id="motor5-title" onclick="testMotor('motor5')" onmouseover="changeText(this, 'Testar Motor?')"
                onmouseout="changeText(this, 'Motor 360')">
                Motor 360
            </h2>
            <div class="slider-values">
                <label>Velocidade: <input type="number" id="motor5-value_vel" value="0" min="200" max="2500" step="1"
                        onclick="updateFromInput_vel('motor5')"></label>
                <label>Angulo Atual: <input type="number" id="motor5-value" value="0" min="-180" max="180" step="1"
                        oninput="updateFromInput('motor5')"></label>
            </div>
            <div class="slider-controls">
                <label>Mínimo: <input type="number" id="motor5-min" value="-180"
                        oninput="updateRange('motor5')"></label>
                <label>Máximo: <input type="number" id="motor5-max" value="180" oninput="updateRange('motor5')"></label>
            </div>
            <div class="angle-visual-container">
                <div class="angle-visual" id="motor5-line"></div>
            </div>
            <label class="slider-label">Controle (graus):</label>
            <input type="range" id="motor5-slider" min="-180" max="180" value="0" oninput="updateValue('motor5')">
        </div>


        <!-- Modal -->
        <div id="confirmModal" class="modal">
            <div class="modal-content">
                <span class="close" onclick="closeModal()">&times;</span>
                <p id="modal-text"></p>
                <button id="confirmButton">Confirmar</button>
                <button id="cancelButton" onclick="closeModal()">Cancelar</button>
            </div>
        </div>

        <footer>
            <p>&copy; 2024 Braçudo - FASTECH</p>
        </footer>

        <script>
            // Função para atualizar o valor e a rotação a partir do slider
            function updateValue(motor) {
                const slider = document.getElementById(motor + '-slider');
                const value = parseFloat(slider.value);
                const valueInput = document.getElementById(motor + '-value');
                valueInput.value = value;

                const min = parseFloat(slider.min);
                const max = parseFloat(slider.max);

                let rotation = 0;
                if (max !== min) {
                    rotation = ((value - min) / (max - min)) * 180 - 90;
                }

                const line = document.getElementById(motor + '-line');
                line.style.transform = `rotate(${rotation}deg)`;
            }

            // Função para atualizar o slider e a rotação a partir do input
            function updateFromInput(motor) {
                const valueInput = document.getElementById(motor + '-value');
                let value = parseFloat(valueInput.value);
                const slider = document.getElementById(motor + '-slider');

                const min = parseFloat(slider.min);
                const max = parseFloat(slider.max);

                if (value < min) {
                    value = min;
                } else if (value > max) {
                    value = max;
                }

                valueInput.value = value;
                slider.value = value;

                let rotation = 0;
                if (max !== min) {
                    rotation = ((value - min) / (max - min)) * 180 - 90;
                }

                const line = document.getElementById(motor + '-line');
                line.style.transform = `rotate(${rotation}deg)`;
            }

            function updateFromInput_vel(motor) {
                const valueInput = document.getElementById(motor + '-value_vel');
                let value = parseFloat(valueInput.value);

                if (value < 200) {
                    value = 200;
                } else if (value > 2500) {
                    value = 2500;
                }

                valueInput.value = value;
            }

            // Função para atualizar o intervalo do slider
            function updateRange(motor) {
                const minInput = document.getElementById(motor + '-min');
                const maxInput = document.getElementById(motor + '-max');
                const slider = document.getElementById(motor + '-slider');
                const valueInput = document.getElementById(motor + '-value');

                slider.min = minInput.value;
                slider.max = maxInput.value;

                valueInput.min = minInput.value;
                valueInput.max = maxInput.value;

                let value = parseFloat(valueInput.value);
                const min = parseFloat(slider.min);
                const max = parseFloat(slider.max);

                if (value < min) {
                    value = min;
                } else if (value > max) {
                    value = max;
                }

                valueInput.value = value;
                slider.value = value;

                updateValue(motor);
            }

            // Função para salvar a configuração atual
            function saveConfiguration() {
                const configName = prompt('Digite um nome para a configuração:');
                if (!configName) {
                    alert('Nome inválido. Configuração não salva.');
                    return;
                }

                // Cria um objeto para armazenar as configurações
                const config = {};

                // Loop através dos 5 motores
                for (let i = 1; i <= 5; i++) {
                    const motor = 'motor' + i;

                    // Obtém os valores atuais, mínimos e máximos
                    const value_vel = parseFloat(document.getElementById(motor + '-value_vel').value);
                    const value = parseFloat(document.getElementById(motor + '-value').value);
                    const min = parseFloat(document.getElementById(motor + '-min').value);
                    const max = parseFloat(document.getElementById(motor + '-max').value);

                    // Armazena os valores no objeto de configuração
                    config[motor] = {
                        value_vel: value_vel,
                        value: value,
                        min: min,
                        max: max
                    };
                }

                // Recupera todas as configurações existentes ou inicia um novo objeto
                const allConfigs = JSON.parse(localStorage.getItem('allMotorConfigurations')) || {};

                // Adiciona a nova configuração
                allConfigs[configName] = config;

                // Salva todas as configurações atualizadas
                localStorage.setItem('allMotorConfigurations', JSON.stringify(allConfigs));

                // Atualiza o menu suspenso
                updateConfigurationsDropdown();

                alert('Configuração "' + configName + '" salva com sucesso!');
            }

            // Função para carregar a configuração selecionada
            function loadSelectedConfiguration() {
                const dropdown = document.getElementById('configurations-dropdown');
                const configName = dropdown.value;

                if (!configName) {
                    alert('Por favor, selecione uma configuração.');
                    return;
                }

                const allConfigsString = localStorage.getItem('allMotorConfigurations');
                if (allConfigsString) {
                    const allConfigs = JSON.parse(allConfigsString);

                    if (allConfigs[configName]) {
                        const config = allConfigs[configName];

                        // Loop através dos motores e aplica os valores
                        for (let i = 1; i <= 5; i++) {
                            const motor = 'motor' + i;

                            if (config[motor]) {
                                const value_vel = config[motor].value_vel;
                                const value = config[motor].value;
                                const min = config[motor].min;
                                const max = config[motor].max;

                                // Atualiza os campos de mínimo e máximo
                                document.getElementById(motor + '-min').value = min;
                                document.getElementById(motor + '-max').value = max;
                                updateRange(motor);

                                // Atualiza o valor atual
                                document.getElementById(motor + '-value_vel').value = value_vel;
                                document.getElementById(motor + '-value').value = value;
                                document.getElementById(motor + '-slider').value = value;
                                updateValue(motor);
                            }
                        }
                    } else {
                        alert('Configuração não encontrada.');
                    }
                } else {
                    alert('Nenhuma configuração salva encontrada.');
                }
            }

            // Função para atualizar o menu suspenso com as configurações salvas
            function updateConfigurationsDropdown() {
                const dropdown = document.getElementById('configurations-dropdown');
                dropdown.innerHTML = '<option value="">Selecione uma Configuração</option>';

                const allConfigsString = localStorage.getItem('allMotorConfigurations');
                if (allConfigsString) {
                    const allConfigs = JSON.parse(allConfigsString);
                    for (const configName in allConfigs) {
                        const option = document.createElement('option');
                        option.value = configName;
                        option.textContent = configName;
                        dropdown.appendChild(option);
                    }
                }
            }

            function changeText(element, newText) {
                element.innerHTML = newText;
            }
            function testMotor(motor) {
                const value_vel = parseFloat(document.getElementById(motor + '-value_vel').value);
                const value = parseFloat(document.getElementById(motor + '-value').value);
                let sentido = "Direita";
                if (value < 0) {
                    sentido = "Esquerda";
                } else if (value == 0) {
                    sentido = "Nao meche";
                }
                const isConfirmed = confirm("Tem certeza que deseja testar o " + motor + "?\n" +
                    "Velocidade: " + value_vel + "\n" +
                    "Sentido: " + sentido + "\n" +
                    "Qntd Rotação: " + Math.abs(value)
                );
                if (isConfirmed) {
                    // Criar um objeto com os dados do motor
                    let motorData = {};
                    motorData[motor] = {
                        value_vel: value_vel,
                        value: Math.abs(value),
                        sentido: sentido
                    };

                    // Converter o objeto em uma string JSON
                    let motorDataString = JSON.stringify(motorData);

                    // Enviar os dados para o ESP8266 usando fetch
                    fetch('/testMotor', {
                        method: 'POST',
                        headers: {
                            'Content-Type': 'application/json'
                        },
                        body: motorDataString
                    })
                    .then(response => response.text())
                    .then(data => {
                        alert("Comando de teste enviado com sucesso!");
                    })
                    .catch(error => {
                        alert("Erro ao enviar comando de teste!");
                        console.error(error);
                    });
                } else {
                    alert("Teste cancelado.");
                }
            }
// Declare as variáveis no escopo global
let config = {};
let configString = "";

function Subir_todos() {
    // Resetar as variáveis ao chamar a função
    config = {};
    configString = "";

    // Inicializa o objeto config
    for (let i = 1; i <= 5; i++) {
        const motor = 'motor' + i;
        const value_vel = parseFloat(document.getElementById(motor + '-value_vel').value);
        const value = parseFloat(document.getElementById(motor + '-value').value);

        let sentido = "Direita";
        if (value < 0) {
            sentido = "Esquerda";
        } else if (value == 0) {
            sentido = "Nao meche";
        }

        config[motor] = {
            value_vel: value_vel,
            value: Math.abs(value),
            sentido: sentido
        };
    }

    // Construir a string do conteúdo 
    for (let motor in config) {
        configString += motor + ":\n";
        configString += "  Velocidade: " + config[motor].value_vel + "\n";
        configString += "  Rotação: " + config[motor].value + "\n";
        configString += "  Sentido: " + config[motor].sentido + "\n\n";
    }

    // Exibir o modal com o conteúdo
    document.getElementById('modal-text').innerText = configString;
    openModal();    
}

// Função para abrir o modal
function openModal() {
    document.getElementById('confirmModal').style.display = 'block';
}

// Função para fechar o modal
function closeModal() {
    document.getElementById('confirmModal').style.display = 'none';
}

// Função para confirmar a ação
document.getElementById('confirmButton').onclick = function() {
    // Converter o objeto config em uma string JSON
    let configJSON = JSON.stringify(config);

    fetch('/setMotor', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: configJSON
    })
    .then(response => response.text())
    .then(data => {
        alert("Comandos enviados com sucesso!");
    })
    .catch(error => {
        alert("Erro ao enviar comandos!");
        console.error(error);
    });
    closeModal(); // Fecha o modal após a confirmação
};


            // Função para abrir o modal
            function openModal() {
                document.getElementById('confirmModal').style.display = 'block';
            }

            // Função para fechar o modal
            function closeModal() {
                document.getElementById('confirmModal').style.display = 'none';
            }

            // Chama a função para atualizar o menu suspenso ao carregar a página
            window.onload = function() {
                updateConfigurationsDropdown();
            };
        </script>

    </div> <!-- Fecha o main-container -->
</body>

</html>
)rawliteral";

void handleSetMotor() {
  if (server.method() == HTTP_POST) {
    String motorData = server.arg("plain");

    // Imprimir os valores recebidos no terminal serial do ESP8266
    Serial.println("Dados recebidos do site:");
    Serial.println(motorData);

    // Envia os dados para o Mega via comunicação serial
    megaSerial.println("COMANDO:" + motorData);

    // Retorna uma resposta de sucesso para o navegador
    server.send(200, "text/plain", "Comandos enviados ao Mega!");
  } else {
    server.send(405, "text/plain", "Método não permitido");
  }
}

// Função para processar a requisição HTTP POST para testes de motor
void handleTestMotor() {
  if (server.method() == HTTP_POST) {
    String motorData = server.arg("plain");

    // Imprimir os valores recebidos no terminal serial do ESP8266
    Serial.println("Dados de teste recebidos do site:");
    Serial.println(motorData);

    // Envia os dados para o Mega via comunicação serial
    megaSerial.println("TESTE:" + motorData);

    // Retorna uma resposta de sucesso para o navegador
    server.send(200, "text/plain", "Comando de teste enviado ao Mega!");
  } else {
    server.send(405, "text/plain", "Método não permitido");
  }
}

// Função para servir o HTML
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void setup() {
  Serial.begin(115200);          // Monitor Serial
  megaSerial.begin(9600);        // Comunicação com o Arduino Mega

  // Conecte-se ao Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Conectando ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado ao WiFi. IP: ");
  Serial.println(WiFi.localIP());

  // Inicia o servidor e define as rotas
  server.on("/", handleRoot);
  server.on("/setMotor", HTTP_POST, handleSetMotor);
  server.on("/testMotor", HTTP_POST, handleTestMotor); // Nova rota para teste

  server.begin();
  Serial.println("Servidor iniciado.");
}

void loop() {
  server.handleClient();  // Processa solicitações do cliente
}
