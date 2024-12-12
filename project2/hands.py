import cv2
import mediapipe as mp

# Inicialização da câmera (ajuste o índice se necessário)
video = cv2.VideoCapture(0)

# Inicialização do Mediapipe Hands
hands = mp.solutions.hands
Hands = hands.Hands(max_num_hands=1)
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
                print("Esperando sinal...")
            elif contador == 0:  # Mão fechada
                print("Desligando o sistema")
                video.release()
                cv2.destroyAllWindows()
                exit()  # Fecha a execução do programa
            elif contador == 1:  # Apenas dedo indicador levantado
                print("Aumentando a temperatura")
            elif contador == 2:  # Indicador e médio levantados
                print("Diminuindo a temperatura")

    # Exibindo o vídeo
    cv2.imshow('Imagem', img)
    if cv2.waitKey(1) & 0xFF == ord('q'):  # Pressione 'q' para sair
        break

# Liberando recursos
video.release()
cv2.destroyAllWindows()
