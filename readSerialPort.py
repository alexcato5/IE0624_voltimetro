import serial
import pandas as pd
from pathlib import Path

# Nombre del archivo y ruta donde se guardarán los resultados
nombre_archivo = 'resultados'
ruta_resultados = './' + nombre_archivo + '/'
Path(ruta_resultados).mkdir(parents=True, exist_ok=True)

# Definición del DataFrame para guardar los resultados
formato = { 'V1':[0.00], 'V2':[0.00], 'V3':[0.00], 'V4':[0.00], 'AC':[0] }
resultados_df = pd.DataFrame(formato)

lectura = [] # Lista donde se guarda el número leído
escritura = [] # Lista donde se guardarán los 4 valores leídos
caracter_decimal = False
caracteres_restantes = 2

puerto = serial.Serial('COM2')

while True:
	caracter_leido = puerto.read()
	caracter_leido = caracter_leido.decode('utf-8')
	lectura.append(caracter_leido)

	if caracter_leido == '.':
		# Se leerá todo lo que esté antes de punto, y dos caracteres más
		caracteres_restantes = 2
		caracter_decimal = True
	elif caracter_decimal:
		caracteres_restantes = caracteres_restantes - 1

	if caracteres_restantes == 0:
		caracter_decimal = False
		caracteres_restantes = 2
		escritura.append(float(''.join(lectura)))
		lectura.clear()

		if len(escritura) == 4:
			# Se lee ahora el modo de lectura (AC/DC)
			caracter_leido = puerto.read()
			caracter_leido = caracter_leido.decode('utf-8')
			if caracter_leido == '1':
				escritura.append('AC')
			else:
				escritura.append('DC')

			resultados_df.loc[len(resultados_df.index)] = escritura
			print(escritura)
			escritura.clear()

			# Guardar resultados en formato CSV
			resultados_df.to_csv(ruta_resultados + nombre_archivo + '.csv', encoding='utf-8', index=False)