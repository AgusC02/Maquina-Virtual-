MOV w[2], L[10] ;copia el byte 12 al 2 y el 13 al 3 del Data segment.

Primero tengo que leer [10]
Si es L guardo todo completo en el operando, debo utilizar corrimientos y suma. 
Si es w guardo dos bytes en el operando, con un corrimiento y suma.
Si es b guardo en el operando, se guardará solo un byte.
Si es w o b se propaga solo a int. Entonces no hace falta propagar.

Para la escritura debo posicionarme desde la posición de memoria 2 y moverme byte a byte en memoria con la lectura dependiendo que modificador tenga la lectura. 

Se escribe desde el byte mas significativo por el cual tengo que hacer corrimiento hacia la izquierda dependiendo de donde quiero arrancar. Valor << modif*8
Y lo voy corriendo hacia la izquierda n veces depende el modificador del operando a escribir.










