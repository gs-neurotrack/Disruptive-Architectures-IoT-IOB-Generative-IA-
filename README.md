# 🟦 WorkSense – Monitor IoT de Ambiente de Trabalho Inteligente

O **WorkSense** é um sistema IoT desenvolvido para monitorar temperatura e presença em ambientes de trabalho, enviando dados em tempo real para um dashboard no Node-RED via MQTT.  
A solução demonstra como sensores podem ser utilizados para promover **bem-estar, conforto térmico e segurança**, alinhado ao tema **O Futuro do Trabalho**.

---

# 📌 1. Descrição Completa da Solução

O WorkSense combina **ESP32 + sensores físicos + MQTT + Node-RED** para criar um ambiente de trabalho inteligente.

## 🔧 Dispositivo IoT (ESP32)
Sensores utilizados:

- **Sensor NTC (KY-013)** → mede temperatura ambiente  
- **Sensor PIR** → detecta presença/movimento  
- **LED indicador** → acende quando presença é detectada  

O ESP32 publica pacotes JSON no tópico MQTT:

```json
{
  "temperature": 23.5,
  "presence": 1
}
```

---

## 🔄 Gateway / Middleware – Node-RED

Responsável por:

- Receber dados MQTT  
- Converter JSON  
- Formatar informações  
- Atualizar o dashboard em tempo real  

---

## 📊 Dashboard

Exibe:

- **Temperatura atual (Gauge)**  
- **Indicador de presença**  
- **Histórico de temperatura (gráfico)**  

---

# 📌 2. Arquitetura do Sistema

```
[NTC]        [PIR]
   \          /
    \        /
     --> ESP32 --> MQTT (test.mosquitto.org)
                        |
                        v
                  Node-RED Gateway
                        |
                        v
               Dashboard em tempo real
```

---

# 📌 3. Componentes Utilizados

## Hardware
- ESP32 DevKit V1  
- Sensor NTC KY-013  
- Sensor PIR  
- LED verde  
- Jumpers  

## Software
- Wokwi (simulador ESP32)  
- Node-RED  
- Node-RED Dashboard  
- Broker MQTT público: `test.mosquitto.org`  

---

# 📌 4. Configuração e Execução

---

## 🟦 4.1 ESP32 no Wokwi

1. Acesse http://wokwi.com  
2. Importe o projeto contendo:
   - `diagram.json`
   - `sketch.ino`
3. Em `sketch.ino` configure o Wi-Fi:

```cpp
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";
```

4. Clique em **Start Simulation**  
5. Abra o **Serial Monitor** para acompanhar logs

---

## 🟦 4.2 Executando o ESP32

O código do ESP32:

- Conecta ao Wi-Fi  
- Conecta ao broker MQTT  
- Lê temperatura  
- Lê presença  
- Atualiza LED  
- Publica JSON a cada 3 segundos  

---

## 🟦 4.3 Instalando Node-RED

No PowerShell como administrador:

```powershell
Set-ExecutionPolicy RemoteSigned
```

Depois instale:

```powershell
npm install -g --unsafe-perm node-red
```

Inicie:

```powershell
node-red
```

Acesse:

```
http://localhost:1880
```

---

## 🟦 4.4 Instalando o Dashboard

Na pasta `.node-red`:

```powershell
npm install node-red-dashboard
```

---

## 🟦 4.5 Flow do Node-RED (importar)

Menu → **Import** → cole o flow JSON.  
Depois clique em **Deploy**.

Dashboard:

```
http://localhost:1880/ui
```

---

# 📌 5. Execução e Testes

## 🟢 Teste 1 — Temperatura
No Wokwi:

- Clique no sensor NTC  
- Mexa no slider de temperatura  

O gauge atualiza imediatamente  
O gráfico registra o histórico  

---

## 🟢 Teste 2 — Presença
No PIR:

- Altere o campo “State: LOW/HIGH”  

Dashboard exibe:

- **Presença detectada**  
- **Sem presença**

E o LED acende quando presença = 1.

---

## 🟢 Teste 3 — MQTT
No Node-RED:

- Nó “WorkSense MQTT” deve estar verde (conectado)  
- Você pode adicionar um nó Debug para visualizar mensagens recebidas  

---

# 📌 6. Fluxo do Gateway (Node-RED) — Explicação

## 🔴 1. MQTT IN
- Tópico: `workSense/data`  
- Recebe dados enviados pelo ESP32  

---

## 🟠 2. JSON Parser
Converte texto → JSON:

```json
{ "temperature": 27.3, "presence": 0 }
```

---

## 🟡 3. Function – Formata Presença

```js
var presence = msg.payload.presence || 0;
msg.payload = presence === 1 ? "Presença detectada" : "Sem presença";
return msg;
```

---

## 🟢 4. Gauge de Temperatura
Exibe a temperatura atual.

---

## 🔵 5. Texto de Presença
Mostra:

- “Presença detectada”
- “Sem presença”

---

## 🟣 6. Gráfico Histórico da Temperatura
Recebe somente valores numéricos (temperatura), extraídos pelo Function:

```js
msg.payload = msg.payload.temperature;
return msg;
```

---

# 📌 7. Conclusão

O WorkSense demonstra uma solução IoT **simples, funcional e alinhada ao Futuro do Trabalho**, oferecendo:

- Coleta de dados via sensores reais  
- Envio MQTT  
- Processamento no Node-RED  
- Dashboard em tempo real  
 
