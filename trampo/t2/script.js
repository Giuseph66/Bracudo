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

    // Loop através dos 6 motores
    for (let i = 1; i <= 5; i++) {
        const motor = 'motor' + i;

        // Obtém os valores atuais, mínimos e máximos
        const value_vel = parseFloat(document.getElementById(motor + '-value_vel').value);
        const value = parseFloat(document.getElementById(motor + '-value').value);
        const min = parseFloat(document.getElementById(motor + '-min').value);
        const max = parseFloat(document.getElementById(motor + '-max').value);

        // Armazena os valores no objeto de configuração
        config[motor] = {
            value_vel:value_vel,
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

            //alert('Configuração "' + configName + '" carregada com sucesso!');
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
    let sentido="Direita"
    if (value<0){
        sentido="Esquerda"
    }else if(value==0){
        sentido="Nao meche"
    }
    const isConfirmed = confirm("Tem certeza que deseja testar o " +motor+" ?\n"
        +"Velocidade: "+value_vel +"\n"
        +"Sentido: "+sentido+"\n"
        +"Qntd Rotação: "+Math.abs(value)
    );
    if (isConfirmed) {
        alert("Foi"); // Chama a função de teste do motor se o usuário confirmar
    } else {
        alert("Teste cancelado.");
    }
}function Subir_todos() {
    let config = {}; // Inicializa o objeto config
    
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
    let configString = "";
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
    alert("Foi");
    closeModal(); // Fecha o modal após a confirmação
};


// Chama a função para atualizar o menu suspenso ao carregar a página
window.onload = function() {
    updateConfigurationsDropdown();
};
