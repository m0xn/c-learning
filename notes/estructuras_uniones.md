# Uniones y estructuras en C

## Artículos consultados

- https://www.programiz.com/c-programming/c-unions
- https://www.programiz.com/c-programming/c-structures
- https://en.cppreference.com/w/c/language/struct_initialization

## Introducción estructuras y uniones

Al contrario que la mayoría de los lenguajes de programación, C no está enfocado a la programación orientada
a objetos aunque cuenta con algunos elementos que facilitan acercase a este paradigma.

La forma que ofrece C para codificar **tipos de datos derivados o definidos por el usuario**, son las **estructuras**
y las **uniones**. Ambas comparten la función de crear **tipos de datos más complejos**, pero difieren en la forma
en la que se almacenan memoria.

Estructuras y uniones en última instancia son tipos de datos definidos por el usuario que están formado por
una serie de **miembros o propiedades** que pueden ser de **tipos primitivos** (*`char`, `int`, `float`...*) o bien pueden
tener por tipo otra **estructura o unión**.

## Definición sintáctica de las estructuras y uniones

Para declarar estructuras en C, se utiliza la palabra clave `struct`. La sintaxis para definir una estructura
sigue la forma:

```c
struct nombre_estructura {
    tipo_miembro1 nombre_miembro1;
    tipo_miembro2 nombre_miembro2;
    ...
};
```

Donde "nombre_estructura" es el nombre que se le asigna a la estructura definida y que va a ser utilizado para
crear variables de ese tipo de estructura, `tipo_miembro#` es el tipo del miembro de la estructura que
estamos definiendo y "nombre_miembro#" es el nombre que recibe dicho miembro y al que haremos referencia a la hora
de obtener o modificar su valor actual.

De una forma similar, se pueden definir las uniones en C, utilizando la palabra clave `union`. La sintaxis para
definir una unión sigue la forma:

```c
union nombre_union {
    tipo_miembro1 nombre_miembro1;
    tipo_miembro2 nombre_miembro2;
    ...
};
```

Lo único que cambia en cuanto a su definición es la palabra clave `union`, el resto permanece igual.

## Declaración de variables de tipo estructura o unión

Una vez que se ha definido una estructura o unión, se pueden crear variables de tipo estructura o unión
utilizando la siguiente forma:

```c
struct nombre_estructura nombre_variable;
union nombre_union nombre_variable;
```

> [!IMPORTANT]
> Nótese que las palabras clave `struct` y `union` se utilizan junto con el nombre de la estructura o unión
> correspondiente para la declaración de una variable de dicho tipo. *En la siguiente sección veremos otra
> forma de definir estructuras y uniones de modo que podamos prescindir de las palabras clave para declarar una
> variable de tipo estructura o unión*.

Otra forma acortada de declarar variables de estructura o unión es la siguiente:

```c
struct nombre_estructura {
    tipo_miembro1 nombre_miembro1;
    tipo_miembro2 nombre_miembro2;
    ...
} nombre_variable;

union nombre_union {
    tipo_miembro1 nombre_miembro1;
    tipo_miembro2 nombre_miembro2;
    ...
} nombre_variable;
```

De este modo también se puede prescindir de reescribir las palabras clave `struct` y `union` cuando queramos
declarar una nueva variable de ese tipo.

## Definiciones de tipo y uso de `typdef`

En C, existe una palabra clave `typdef` que cumple la función de asignar un **alias** o **pseudónimo**
a un tipo de dato que bien pueder ser primitivo o más frecuentemente será complejo como lo son las **estrucutras** y las **uniones**.

Esta funcionalidad es especialmente útil para poder declarar variables a posteriori de tipo estructura o unión
sin necesidad de utilizar las palabras clave que preceden a sus nombres, sólo los nombres asignados a las estructuras o
uniones respectivamente.

Para definir una estructura con un alias de tipo, se utiliza la siguiente estructura sintáctica: 

```c
typedef struct nombre_estructura {
    tipo_miembro1 nombre_miembro1;
    tipo_miembro2 nombre_miembro2;
    ...
} alias_estructura;
```

En este caso, al contrario que en la declaración de variables junto a la declaración de la estructura,
el nombre que se especifica tras la llave que cierra la definición (*`alias_estructura`*), es el alias de tipo que se va a utilizar
para declarar variables de tipo estructura de la siguiente manera:

```c
nombre_estructura nombre_variable;
/* Anteriormente era `struct nombre_estructura nombre_variable;` */
```

Para el caso de las uniones, la estructura sintáctica es la misma, sólo cambia la palabra clave `struct`->`union`.
De modo que resultaría algo de la siguiente manera:

```c
typedef union nombre_union {
    tipo_miembro1 nombre_miembro1;
    tipo_miembro2 nombre_miembro2;
    ...
} alias_union;

nombre_union nombre_variable;
```

## Union vs Struct 

Pese a ser muy similares funcionalmente, uniones y estructuras presentan una diferencia clave en cuanto
al almacenamiento que se dedica para alojar sus miembros en memoria.

En el caso de las estructuras, una vez se declara una estructura se aloja en memoria la capacidad necesaria
para almacenar todos los miembros de la estructura.
Véase la siguiente estructura:

```c
struct Person {
    char name[32];
    int age;
    float salary;
}
```

Teniendo en cuenta que el tipo primitivo `char` ocupa un tamaño de **1 byte** en memoria y tenemos un array de 32
elementos, `name` ocupa **32 bytes** en memoria y `age` y `salary` de tipo `int` y `float` respectivamente, ambos ocupan
**4 bytes** en memoria cada uno. Por lo que su suma de **40 bytes** en total.

Por el contrario, en caso de declarar `Person` como una unión:

```c
union Person {
    char name[32];
    int age;
    float salary;
}
```

Pese a que los miembros son los mismos y a priori cabría pensar que la unión ocupa los mismos 40 bytes en memoria, no 
es la forma real en la que funciona este tipo derivado de C.
En C, para las uniones se almacena un bloque de memoria con el tamaño del miembro más grande que contiene la unión.
Esto es, en este caso, el miembro de mayor tamaño es el array de caracteres (*un string de C*) `name`, ya que ocupa un total
de 32 bytes en memoria. Este mismo bloque de memoria, será compartido con los miembros restantes de la unión, `age` y `float`.
La limitación inmediata de esta forma almacenamiento de memoria, es que sólo se puede conservar el valor de uno de los miembros
a la vez. Esto implica que los valores que modificamos en cada uno de los miembros sólo persisten hasta que se modifica otro
de los miembros. Por ejemplo:

```c
typedef union Person {
    char name[32];
    int age;
    float salary;
} Person;

int main()
{
    Person p;

    p.age = 10;
    printf("age: %d, salary: %.2f\n", p.age, p.salary);

    p.salary = 1000.00; /* la miembro `age` deja de tener el valor 10 porque se modifica otro miembro */
    printf("age: %d, salary: %.2f\n", p.age, p.salary);
}
```

En este fragmento de código la primera expresión de impresión a la consola, nos daría una salida con el valor
de `age` puesto en `10` como se había especificado en la asignación anterior y el valor de `salary` como un número
aleatorio que se asigna por defecto a variables sin incializar. Después, al asignar el valor `1000.00` a `salary` puesto
que estamos modificando otro miembro, el valor que se había asignado anteriormente a `age` se pierde y en su lugar
lo sustituye un valor aleatorio como el que se otrogó a `salary` al incio.

*La notación utiliza para acceder a los miembros se va a explicar a continuación*.

## Acceso a los miembros de una estructura o unión

Esta parte es común a ambos tipos de datos derivados, por lo que me estaré refiriendo únicamente a la estructuras
para acortar el contenido de las notas, sin embargo es aplicable a las uniones indistintamente.

Podemos encontrar **2 tipos de notación** para acceder a los miembros de una estructura:

- `.` *notación de punto*.
- `->` *notación de flecha*.

Por ejemplo, en el caso de la notación de punto:

```c
typedef struct Person {
    char name[32];
    int age;
    float salary;
} Person;

int main()
{
    Person p;

    p.age = 10; /* Incializa el valor del miembro age a 10 */ 
    printf("%d\n", p.age); /* Muestra por consola el valor del miembro age, 10 */
}
```

Como contamos con el **valor de la estructura** que almacena la variable `p` podemos acceder y modificar
sus miembros utilizando la **notación de punto `.`**. Lo mismo sucede al pasar una estructura por valor como
argumento de una función que cuente con un parámetro de tipo estructura. Sin embargo, en el caso de arriba
la modificación se aplica sobre `p` y en el mencionado posteriormente la modificación se aplica sobre la copia
local que se hace del valor del estructura en el parámetro.
En esta nota hablo más en detalle acerca del paso de objetos por referencia y por valor -> [Punteros en C](./punteros.md)

Por ejemplo, para el caso de la notación de flecha:

```c
typedef struct Person {
    char name[32];
    int age;
    float salary;
} Person;

int main()
{
    Person p;
    Person *ptr = &p;

    p.age = 20;
    ptr->age = 10; /* Incializa el valor del miembro age a 10 */ 
    printf("%d\n", ptr->age); /* Muestra por consola el valor del miembro age, 10 */
}
```

Como contamos con un puntero que apunta hacia la dirección de memoria donde se almacena la estructura,
para poder acceder y modificar los miembros de dicha estructura se ha de utilizar la notación de flecha `->`.

En el caso mencionado anteriormente, la notación de flecha sería equivalente a la expresión: `(*ptr).age`.

## Incialización de estructuras 

Este último apartado es sólo aplicable a las estructuras, puesto que las uniones comparten un mismo bloque
de memoria y por tanto no tiene sentido inicializar todos los miembros al momento de declarar la variable de
tipo unión.

En lugar de tener que referenciar y modificar los miembros de una variable de tipo estructura utilizando
la notación de punto `.` mencionada anteriormente, en C existe una forma de incializar todos los miembros o
sólo algunos de ellos al momento de declarar una variable de tipo estructura.

Para ello se utiliza una estructura sintáctica similar a la que se emplea para definir los elementos de un array.
La forma más básica de incializar los valores de una estructura es utilizando la construcción sintáctica de una lista
entre llaves de valores. De modo que teniendo en cuenta la estructura `Person`, podríamos declarar e incializar la variable
`p` de la siguiente manera:

```c
typedef struct Person {
    char name[32];
    int age;
    float salary;
} Person;

int main()
{
    Person p = {"Juan", 20, 1000.00};
}
```

Este fragmento de código declara e incializa una variable `p` de tipo `Person` que es un alias de tipo para
la estructura definida en anteriormente, con los valores `"Juan"` para el miembro `name`, `20` para el miembro `age`
y `1000.00` para el miembro `salary`.

> [!IMPORTANT]
> Cabe recalcar que no es necesario incializar todos los miembros de la estructura, pero se ha de tener en cuenta
> que los valores especificados se asignan a los miembros en el orden en el que se definen.

Para ser más explícitos con la incialización de los miembros de la estructura podemos emplear la siguiente notación
para referirnos a cada uno de los miembros por separado:

```c
typedef struct Person {
    char name[32];
    int age;
    float salary;
} Person;

int main()
{
    Person p = {
        .name = "Juan", 
        .age = 20, 
        .salary = 1000.00
    };
}
```

Como se puede observar para hacer referencia a cada uno de los miembros de la estructura se especifica el
nombre de la propiedad al que le precede un punto. De modo por lo general, para acceder a una propiedad de
una estructura emplearemos la notación `.nombre_propiedad`.

En caso de querer especificar **el valor de un elemento de un array**, también existe otra notación que permite
asignar a un elemento de índice `i` de la lista el valor que especifique el desarrollador:

```c
typedef struct Person {
    char name[32];
    int age;
    float salary;
} Person;

int main()
{
    Person p = {
        .name = {
            [0] = 'J',
        },
        .age = 20, 
        .salary = 1000.00
    };
}
```

Por tanto, se utiliza la sintaxis `[i] = valor,` para asignar un valor específico (*del tipo del array que almacene
el miembro que estamos incializando*) al elemento de índice `i`.
