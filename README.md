# Ray-tracing

- Detalles a tener en cuenta:
    * Ni idea de si el código funciona tal cual, pero si no es el caso debes seguir las instrucciones del inicio de: https://raytracing.github.io/books/RayTracingInOneWeekend.html
    * El suelo es una esfera, un poco ridículo lo sé.
    * La cámara puede moverse en todas direcciones mediante un sistema de centro-objetivo, pero no es posible rotarla.
    * El viewport no es donde se ven los píxeles, es simplementye un marco de referencia para lo vectores salientes.

- Tareas:
    * Implementar planos de perimetros limitados.
    * Implementa material de luz.
    * Formato para almacenar objetos de forma externa al código.
    * Paralelizar secciones de imagen mediante OpenMP.
    * Paralelizar generacios de vectores aleatorios por pixel mediante OpenMP.
    * Hacer antialiasing en master.

- Modificaciones importantes respecto a libro:
    * No se ha implementado antialiasing, para simplificar paralelización.
    * Ciertas partes del código han sido alteradas para mayor claridad