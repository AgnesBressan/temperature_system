import cv2
import mediapipe as mp
import serial
import time

# Configuração da porta Serial para comunicação com o Arduino
arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=1)
time.sleep(2)  # Aguarde o Arduino inicializar

# Inicialização da câmera
video = cv2.VideoCapture(0)

# Configuração do tamanho do vídeo para acelerar o processamento
video.set(cv2.CAP_PROP_FRAME_WIDTH, 320)  # Largura
video.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)  # Altura

# Inicialização do Mediapipe Hands
hands = mp.solutions.hands
Hands = hands.Hands(max_num_hands=1, min_detection_confidence=0.7, min_tracking_confidence=0.7)
mpDraw = mp.solutions.drawing_utils

while True:
    success, img = video.read()
    if not success:
        print("Erro: Não foi possível capturar o frame da câmera.")
        break

    # Convertendo o frame para RGB
    frameRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = Hands.process(frameRGB)
    handPoints = results.multi_hand_landmarks
    h, w, _ = img.shape
    pontos = []

    if handPoints:
        for points in handPoints:
            mpDraw.draw_landmarks(img, points, hands.HAND_CONNECTIONS)
            for id, cord in enumerate(points.landmark):
                cx, cy = int(cord.x * w), int(cord.y * h)
                pontos.append((cx, cy))

            # Identificando os dedos levantados
            dedos = [8, 12, 16, 20]
            contador = 0
            if pontos:
                # Verifica os dedos (indicador, médio, anelar, mínimo)
                for x in dedos:
                    if pontos[x][1] < pontos[x - 2][1]:  # Se o dedo estiver acima da junta anterior
                        contador += 1

            # Exibindo o resultado
            cv2.rectangle(img, (10, 10), (450, 110), (255, 0, 0), -1)
            cv2.putText(img, f'Gesto Detectado: {contador}', (20, 80), cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 255), 3)

            # Realizando ações com base no gesto
            if contador == 5:  # Mão aberta
                print("Esperando sinal...")  # Mensagem no terminal
                arduino.write(b'W')  # Opcional: envie um comando para o Arduino se necessário
            elif contador == 0:  # Mão fechada
                print("Desligando o sistema")
                video.release()
                cv2.destroyAllWindows()
                arduino.close()  # Fecha a conexão com o Arduino
                exit()  # Fecha a execução do programa
            elif contador == 1:  # Apenas dedo indicador levantado
                print("Aumentando a temperatura")
                arduino.write(b'A')  # Envia o comando 'A' para aumentar a temperatura no Arduino
            elif contador == 2:  # Indicador e médio levantados
                print("Diminuindo a temperatura")
                arduino.write(b'D')  # Envia o comando 'D' para diminuir a temperatura no Arduino

    # Exibindo o vídeo
    cv2.imshow("Imagem", img)
    if cv2.waitKey(1) & 0xFF == ord('q'):  # Pressione 'q' para sair
        break

# Liberando recursos
video.release()
cv2.destroyAllWindows()
arduino.close()
