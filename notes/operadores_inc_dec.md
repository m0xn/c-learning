# Operadores de incremento y decremento

Como se indica en el mítico libro de (*The C Programming Language | Brian W. Kernighan, Dennis M. Ritchie*),
los operadores de incremento (++) y decremento (--) en el lenguaje C se pueden utilizar como prefijos (++x o --x)
o bien como sufijos (x++ y x--) siendo x una variable cualquiera (*Precisamente tiene que ser una variable
porque no está permitido en el lenguaje incrementar o decrementar un constante numérica en una ud.*):

Por ejemplo:

```c
int x;

x = 0;
++x; /* x = x + 1 <-> x += 1; que resultaría en x teniendo un valor de 1 */
--x; /* x = x - 1 <-> x -= 1; que resultaría en x teniendo un valor de 0 (porque x = 1 previamente)*/
(x+1)++ /* Esta operación daría un error en tiempo de compilación porque no está permitida por el lenguaje */
```

Cuando se trata de modificar el valor de la variable y no se está utilizando dentro de otra expresión, se puede
utilizar intercambiablemente la notación de prefijo y de sufijo (*++i = i++*).
Sin embargo, cuando se está utilizando la variable dentro de una expresión, hay que tener en cuenta que el orden
de las operaciones varía de un uso a otro.
De modo que dado, el siguiente fragmento de código:

```c
int x, n;

n = 5;
x = 0;

x = n++; /* x = 5, n = 6 */
// Suponiendo que sólo una de las expresiones se ejecutan
x = ++n; /* x = 6, n = 6 */
```

Para el primer caso, primero se realiza la operación de asignación del valor de la variable `n` en la variable
`x` y posteriormente, se incrementa el valor de la variable `n` en una ud.
De modo que la primera operación de asignación sería equivalente a lo siguiente:

```c
int x, n = 5;

x = n;
n = n + 1; /* n += 1 */
```

Por tanto, en este caso el order de operaciones va en de la siguiente forma:
**asigna a x el valor de n -> suma a la variable n una ud.**

Para el segundo caso, primero se realiza la operación de incremento sobre la variable `n` y después se asigna el
valor de `n` a la variable `x`. Esto quiere decir, que como el orden de operaciones se ha invertido, el valor
que toma en última instancia la variable `x` es el nuevo valor que ha adquirido la variable `n`.
De modo que la segunda operación de asignación sería equivalente a lo siguiente:

```c
int x, n = 5;

n = n + 1; /* n += 1 */
x = n;
```

Por tanto en este caso, el orden de las operaciones se encuentra revertido:
**suma a la variable n una ud.->asigna a x el valor de n**

En conclusión, la forma de prefijo y sufijo de los operadores de incremento y decremento puede ser intercambiable cuando
la variable no se está empleando en ninguna expresión, pero se ha de tener muy en cuenta el orden de operaciones en el
caso contrario.
