# Punteros en C 

Link al artículo original -> https://www.geeksforgeeks.org/c-pointers/

## ¿Qué es un puntero?

Los punteros son un tipo de variable que almacena una **dirección de memoria**. Esta dirección de memoria
puede ser la dirección de memoria donde se almacena otra variable, una función, una estructura o incluso otro
puntero.

La sintaxis fundamental de un puntero en C es la siguiente:
```c
tipo_dato *nombre_puntero;
```
Donde encontramos `tipo_dato` que hace referencia al tipo de dato que tiene la variable a la que va a estar 
apuntando el puntero en memoria. El **operador de desreferenciación** (\*) que explicaré más adelante. Y por último
el nombre que recibe el puntero "nombre_puntero".

Los punteros nos permiten almacenar direcciones de memoria y por consiguiente acceder a los valores que almacenan
y modificarlos.

## ¿Qué tamaño tiene un puntero?

El tamaño de un puntero es **independiente al tipo de dato de la variable a la que está apuntando en memoria**, puesto que
el puntero almacena una dirección de memoria, independientemente del valor que esté contenido en ella. De hecho, el
tamaño del puntero, varía en función de la arquitectura. Para poder obtener el tamaño de un puntero en el sistema
actual el fragmento de código siguiente muestra el tamaño de un puntero en bytes:

```c
// Supongamos que el puntero apunta a una variable de tipo entero, aunque
// esto es irrelevante porque el tamaño se mantiene constante para tipos de datos distintos

int *ptr;
printf("%d\n", sizeof(ptr));
```

Para máquinas de arquitectura x64, el tamaño de un puntero es de 8 bytes (*que tiene sentido puesto que
1byte=8bits y 8bytes x 8bits/byte hacen 64bits*). La misma operación se podría llevar a cabo para el resto de 
arquitecturas. De manera que para una máquina de 32 bits, el tamaño del puntero habría de ser de 4 bytes,
puesto que 4bytes x 8bits/byte = 32bits.

## Fases en la creación de un puntero

Podemos distinguir 3 fases en la creación de un puntero:
1. Fase de declaración
2. Fase de incialización
3. Fase de desreferenciación

### 1. Fase de declaración

En esta fase se declara el puntero que va a almacenar la dirección de memoria correspondiente a la variable
a la que se busca apuntar. Siguiendo la sintaxis especificada anteriormente, supongamos que vamos a declarar
un puntero a una variable entera.

```c
int *ptr;
```

Al no inicializar el puntero que ningún valor se le asigna automáticamente una dirección de memoria aleatoria.

### 2. Fase de incialización

En esta fase le asignamos un valor al puntero. **El valor del puntero será una dirección de memoria**. Supongamos
que definimos una variable entera que almacena el valor `10` con el nombre `x`. Para poder acceder a su dirección
de memoria utilizaremos el **operador de dirección de memoria (&)** delante del nombre de la variable para obtener su
dirección en memoria:

```c
int x = 10;
int *ptr = &x;
```

Si se muestra en consola el valor de `ptr` utilizando el *formateador* dedicado para punteros `%p` podemos observar
el valor de la dirección de memoria de `x`, de la siguiente manera:

```c
int x = 10;
int *ptr = &x;
printf("%p\n", ptr); /* Muestra por consola la dirección de memoria de "x" de la forma 0x..... */
```

En el paso siguiente, utilizando el **operador de desreferenciación (\*)**, podremos acceder al valor almacenado en
la dirección de memoria que guarda la variable `ptr`.

### 3. Fase de desreferenciación

Una vez hemos almacenado en el puntero la dirección de memoria de la variable a la que queremos apuntar, podemos
acceder al valor de la variable a través del puntero, sin necesidad de recurrir a la variable propiamente definida.
Para ello, volvemos a utilizar el **operador de desreferenciación (\*)**.

Para poder comparar los resultados y verificar que el valor que se obtiene de desreferenciar el puntero es el mismo
que el de la variable `x` inicializada anteriormente, se puede ejecutar el siguiente fragmento de código:

```c
int x = 10;
int *ptr = &x;

printf("Valor de x = %d\n", x);
printf("Dirección de memoria de x = %p\n", ptr);
printf("Valor almacenado en %p = %d\n", ptr, *ptr);
```

Analicemos el siguiente gráfico:

![Desreferenciación](https://media.geeksforgeeks.org/wp-content/uploads/20230223170531/2.png)

Como se puede observar, el puntero tiene una dirección de memoria (*en este caso 0x7fff98b499e8*).
A su vez este mismo puntero almacena la dirección de memoria de la variable a la que queremos apuntar (*en este caso 0x7fffa0757dd4*).
Al desreferenciar el puntero, se obtiene el valor almacenado en la dirección de memoria correspondiente.

## Tipos de punteros

Atendiendo al tipo de dato al que apunta el puntero, podemos diferenciar los siguientes tipos de punteros:
> [!NOTE]
> Los punteros no tienen por qué apuntar a tipos de datos primitivos (*esto es, tipos como `char`, `int`, `float`, `double`
> entre otros*). También pueden hacerlo a tipos de datos derivados o definidos por el usuario (como el uso de estructuras
> para definir listas enlazadas, árboles binarios, etc.)

### Puntero a un tipo de dato primitivo

Este tipo de punteros apuntan a la dirección de memoria donde se almacena el valor de una variable con un tipo de
dato primitivo. La sintaxis ya he visto anteriormente y es la siguiente:

```c
tipo_dato * nombre_puntero;
```

Por ejemplo:

```c
int x = 10;
int *xptr = &x;

printf("%d\n", x); /* El valor que almacena x */
printf("%p\n", xptr); /* La dirección de memoria de la variable x */
```

*En este caso, el puntero que se define en el ejemplo se llamaría "puntero a un entero"*

### Puntero a un array 

Este tipo de punteros apuntan a la **dirección de memoria donde se almacena el primer elemento de un array**. Puesto
que al **referenciar un array por el nombre** de la variable que lo almacena, se obtiene la **dirección de memoria del 
primer elemento del array**, este puntero equivale a referenciar el array por el nombre. La sintaxis sería la siguiente:

```c
tipo_dato_array *nombre_puntero = nombre_array;
```

Donde `tipo_dato_array` es el tipo de dato de los elementos que ocupan el array, "nombre_puntero" es el nombre que se
le va a asignar al puntero y "nombre_array" que es el nombre del array que hemos definido previamente y a través del
cual **obtenemos la dirección de memoria del primer elemento del array, que queremos guardar en el puntero**.

Por ejemplo:

```c
int nums[3] = {1, 2, 3};
int *numsptr = nums;

// Ambas expersiones hacen referencia a la dirección de memoria del primer elemento del array
printf("%p\n", nums);
printf("%p\n", numsptr);
```

### Puntero a una estructura

Este puntero apunta una estructura de C. Se llaman **"punteros a estructuras"** o "punteros estructura" y apuntan a constructos 
del usuario o precodificados por otras personas para implementar **estructuras de datos personalizadas**, como lo serían 
*listas enlazadas, árboles de decisión, etc*. La sintaxis sería la siguiente:

```c
struct nombre_structura *nombre_puntero;
```

Por el momento no sé mucho sobre las estructuras así que el ejemplo queda por poner.

### Puntero a una función

Este puntero apunta a las funciones. Es diferente al resto del punteros en sentido de que en lugar de almacenar direcciones
de memoria que contienen datos como el resto de punteros vistos anteriomente, **almacenan direcciones de memoria que contien
código**. La sintaxis sería la siguiente:

```c
tipo_retorno (*nombre_puntero)(tipos_argumentos);
```

Donde `tipo_retorno` es el tipo de valor que devuelve la función, "nombre_puntero" es el nombre del puntero que apunta
a la función (*es importante que se encuentre entre paréntesis*) y "tipos_argumentos" serían los **tipos de los argumentos**
que toma la función a la que apunta el puntero.

Por ejemplo:

```c
int sum(int a, int b)
{
    return a + b;
}

int (*sumptr)(int, int) = &sum;
```

*Este tipo de punteros a funciones resultan especialmente útiles para pasar funciones como argumentos de
otra función (callback functions)*

### Punteros múltiples

Este tipo de punteros apuntan a otros punteros. Es decir, apuntan a la dirección de memoria que contiene el puntero
que apunta a su vez a otra dirección de memoria que puede contener un valor u otro puntero. La sintaxis sería la
siguiente (*teniendo en cuenta un puntero doble*):

```c
tipo_dato **nombre_puntero = &nombre_otro_puntero;
```

Donde `tipo_dato` debe ser el tipo de dato al que apunta el puntero incial cuya dirección se está almacenando en el
puntero doble que se define, "nombre_puntero" será el nombre que se le asigna al puntero doble y para asignar la
dirección de memoria del puntero que queremos almacenar en el puntero doble, utilizamos el **operador de dirección
de memoria (&)** visto anteriormente.

En este caso, para poder acceder al valor que almacena el puntero cuya dirección de memoria se está almacenando en
el puntero doble debemos emplear el **operador de desreferenciación 2 veces (\*\*)**. Esto es, la expresión
`nombre_puntero` devolvería la dirección de memoria del puntero que almacena, la expresión `*nombre_puntero` devolvería
la dirección que almacena el puntero conteido en el puntero doble y la expresión `**nombre_puntero` devolvería el valor
contenido en la dirección de memoria del puntero que se asignó al puntero doble.

Se pueden "encadenar" referencias a tantos punteros como se quiera, por tanto, además de dobles, se puede tener
punteros triples, cuádruples, quíntuples y en adelante.

Por ejemplo:

```c
int x = 10;
int *xptr = &x;
int **xptrptr = &xptr;

printf("%p\n", *xptrptr); /* Se obtiene la dirección de memoria de x (&x), que es el valor que almacena xptr*/
printf("%d\n", **xptrptr); /* Se obtiene el valor de x (10)*/ 

/* 
El proceso que se sigue es el siguiente:
1. El puntero xptrptr, almacena la dirección de memoria del puntero xptr
2. Al desreferenciar la dirección de memoria del puntero xptr, se obtiene el valor almacenado en xptr, en este caso
la dirección de memoria de x (&x).
3. Al desreferenciar de nuevo, como se ha obtenido la dirección de memoria de x, se obtendría el valor de x, que
está contenido en esa dirección de memoria. Es decir **xptrptr = *xptr.
*/
```

Imagen de ayuda:
![Dobles punteros](https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fwww.cdn.geeksforgeeks.org%2Fwp-content%2Fuploads%2FDouble-Pointer-in-C.png&f=1&nofb=1&ipt=0b9e333167b3544ec836513ef53c97ffb613c287d24188b2795a2fd55dca2458&ipo=images)

### Punteros NULL 

Este tipo de punteros no almacenan ninguna dirección de memoria. En otras palabras, almacenan un valor nulo, que en
C se representa con la expresión `NULL`. Un puntero nulo puede apuntar a cualquier tipo de dato, no necesita ser
de ningún tipo específico. Es una buena práctica asignar el valor `NULL` a los punteros que ya no están en uso, por
razones de gestión de memoria manualmente.

La sintaxis sería la misma que la de cualquiera de los punteros vistos anteriormente, simplemente el valor que se le
asigna deja de ser una dirección de memoria, para ser simplemente un **valor nulo**.

Por ejemplo:
```c
char c = 'a';
char *cptr = NULL;
```

Lo mismo pasaría por ejemplo con punteros a funciones:
```c
int mult(int a, int b)
{
    return a * b;
}

int (*multptr)(int, int) = NULL;
```

### Punteros "void"

Este tipo de punteros no apuntan a ningún tipo de dato en particular, sólo almacenan una dirección de memoria, como el
resto de punteros. También se les conoce como **punteros genéricos** ya que no restringen el tipo de dato que debe
estar almacenado en la dirección de memoria que contienen. Se puede inferir el tipo de dato al que apunta el puntero,
pero no se puede desreferenciar. La sintaxis sería:

```c
void *nombre_puntero = dirección_memoria;
```

Donde "nombre_puntero" es el nombre que le asigna a el puntero definido, y "dirección_memoria" puede ser cualquier dirección
de memoria válida dentro del segmento en el que se está trabajando.

### Punteros wild

Estos punteros se caracterizan porque no se incializan con ningún valor. Este tipo de punteros pueden resultar peligrosos
dentro del programa al llevar a crasheos, corrupción de datos, etc.

Por ejemplo:
```c
int *x;
char *c;
```

### Punteros constantes

Este tipo de punteros tienen la peculiaridad de ser **inmutables**. Esto es, se les asigna una dirección de memoria en el
momento de incialización que no puede ser cambiada posteriormente, puesto que la variable se ha marcado como constante
con la palabra clave `const`. La sintaxis sería de la siguiente manera:

```c
tipo_dato *const nombre_puntero = dirección;
```

Lo único que cambia con respecto a la definción del resto de punteros es la introducción de la palabra clave `const` indica
que el valor del puntero se debe mantener constante en el tiempo de ejecución del programa.

Por ejemplo:
```c
int x = 10;
int j = 20;
int *const xptr = &x;

xptr = &j; /* Error de segmentación, no se puede modificar una variable constante */
```

### Punteros a constantes

Este tipo de punteros se diferencian de los anteriores en que en este caso, apuntan a **variables constantes**, pero los
punteros en este caso sí que pueden cambiar la dirección de memoria a la que apuntan a lo largo del tiempo de ejecución
del programa. La sintaxis sería de la siguiente manera:

```c
const tipo_dato *nombre_puntero = dirección;
```

En este caso la palabra clave `const` se antepone al tipo de dato `tipo_dato` al que apunta el puntero para indicar
que va a almacenar la dirección de memoria en la que se encuentre una variable constante. 

Por ejemplo:
```c
const int x = 10;
const int j = 20;
const int *ptr = &x;

ptr = &j; /* No da error, porque el puntero ahora sí que es mutable */
x = 20; /* Error de segmentación, porque la variable x es inmutable (const) */ 
```

También podemos encontrar otros tipos de punteros más complejos como lo son el **dangling pointer**, que hace referencia a
un puntero que almacena una dirección de memoria de un bloque de memoria que se ha liberado anteriormente, o el **file pointer**
que hace referencia a un puntero que apunta a un archivo abierto con la función `fopen()` de la librería *"stdlib.h"*.

## Aritmética con punteros

La arimética con punteros indica las operaciones artiméticas legales o válidas dentro de lenguaje C.
Estas operaciones artiméticas son las siguientes:
- Incremento de un puntero.
- Decremento de un puntero.
- Suma de un puntero con un entero.
- Resta de un puntero con un entero.
- Comparación entre punteros.
- Asignación de punteros del mismo tipo.

La aritmética con punteros tiene un uso particular en el recorrido de los bloques de memoria donde se almacenan
los elementos de un array. Los **elementos de un array** están almacenados en **bloques de memoria contiguos**, por
tanto, incrementar o decrementar el puntero al que hace referencia el nombre del array nos permite recorrer
los elementos del mismo.

Véase el siguiente ejemplo:

```c
int nums[3] = {1, 5, 10};

for (int i = 0; i < 3; ++i) {
    printf("%d\n", *(nums+i)); /* *(nums+i) es equivalente a nums[i] */
}
```

Este bucle recorre y muestra los elementos del array `nums`. Sería equivalente también al siguiente bucle:

```c
for (int i = 0; i < 3; ++i) {
    printf("%d\n", nums[i]);
}
```

La diferencia entre ambos, es que el primero accede a los elementos del array recorriendo los bloques de memoria
donde se encuentran almacenados los distintos valores del mismo y el segundo utiliza la notación de índice para
acceder directamente a los elementos del array en el índice `i`.
De ahí, que en el primer ejemplo se tenga que **desreferenciar el puntero** resultante a la operación arimética 
que se realiza entre paréntesis. 

Esto mismo se podría conseguir haciendo una copia del puntero en otra variable `ptr` a la que incrementemos en
una unidad por cada ciclo del bucle. De modo que resultaría el siguiente fragmento de código:

```c
int nums[3] = {1, 5, 10};
int *ptr = nums;

while (*ptr != 0) { /* Suponiendo que el siguiente bloque de memoria a el que almacena el 10 contiene el valor 0 */ 
    printf("%d\n", *(ptr++));
}
```

De hecho, puesto que `ptr` y `nums` en última instancia son punteros apuntando a la misma dirección de memoria,
esto es, a la dirección de memoria donde se almacena el **primer elemento** del array, podemos utilizar la **notación
de índices sobre el propio puntero**. De modo que el siguiente fragmento de código sería correcto:

```c
int nums[3] = {1, 5, 10};
int *ptr = nums;

for (int i = 0; i < 3; ++i) {
    printf("%d\n", ptr[i]); /* puesto que ptr = nums, también se puede utilizar la notación de índice */
}
```

Esta forma de recorrer una array es un **poco convencional**, puesto que no se impone un límite en cuanto a los bloques
de memoria que te estás desplazando al realizar operaciones aritméticas con los punteros. Por tanto, te puedes
pasar del rango de memoria que cubre el array obteniendo valores que no le corresponden. En su lugar, la notación
de índice para los arrays es más segura al limitar el acceso a los elementos del array si el índice se pasa
de la capacidad del array menos una unidad (*puesto que el índice empieza en 0*).
Aún así es buen ejercicio para practicar la artimética con punteros.

## Punteros como argumentos en funciones

En C, el paso de argumentos a funciones se puede realizar de dos maneras:
- **Paso por valor**
- **Paso por referencia**

Al pasar un **argumento por valor** en una función en C, el valor que se pasa a la función se **asigna 
localmente en los parámetros definidos en la función**. Esto quiere decir, que si se pasa una
variable como argumento a una función, lo que **realmente se modifica** en el cuerpo de la función
es la **variable local** que representa el **párametro** de la función, pero la variable que se pasa
nunca se puede llegar a modificar puesto que sólo se está haciendo al valor que almacena. 
Por ejemplo, el siguiente fragmento de código:

```c
int increment(int a, int step)
{
    a += step;
}

int main()
{
    int x = 10;
    increment(x, 2); /* Sólo se está pasando el valor que almacena x, es decir 10 */
    printf("%d\n", x);
    return 0;
}
```

Resultaría en una salida por consola de 10. Esto se debe a que el valor que almacena la variable
x nunca se ha llegado a modificar, puesto que se ha asignado localmente al parámetro a.
En caso de poner `printf()` en la función `increment()` podríamos observar que la variable `a`
que se genera localmente durante la ejecución de la función sí que cambia de valor. 

Por el contrario al **pasar un argumento por referencia**, es posible **modificar** el valor de la
variable que se intentaba modificar con la función anterior. Esto se debe a que en lugar de pasar
una referencia al valor de la variable, se pasa un **puntero con la dirección de memoria que almacena
el valor de la variable**. Debido a esto, pese a que se utiliza el parámetro de la función durante
su ejecución, al **desreferenciar el puntero** que se ha pasado como argumento a dicho parámetro,
se accede al **bloque de memoria** donde se encuentra el **valor de la variable** que se prentende modificar.
Por tanto, la función arreglada para llevar a cabo la tarea que se pretendía sería la siguiente:

```c
int increment(int *a, int step)
{
    *a += step;
}

int main()
{
    int x = 10;
    increment(&x, 2); /* Se pasa la dirección de memoria donde se encuentra el valor de x */ 
    printf("%d\n", x);
    return 0;
}
```

En este caso, se vería la salida de 12 por consola, justo como se prentendía.

## La palabra clave `restrict`

La palabra clave `restrict` otorga al puntero la cualidad de ser el único puntero que apunta
a un objeto y que por tanto, no existen copias del mismo que se puedan como argumento.
Esto informa al compilador de que el puntero es único y por tanto no tiene por qué hacer
comprobaciones innecesarias acerca de otros punteros que apunten a la misma dirección de memoria. 
