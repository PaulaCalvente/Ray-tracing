# Ray-tracing

- Cosas a tener en cuenta:
    * El código puede no funcionar tal cual, en tal caso debes seguir las instrucciones del inicio de: https://raytracing.github.io/books/RayTracingInOneWeekend.html
    * No hay un suelo, por lo que se debe crear mediante un objeto extra, un poco ridículo lo sé.
    * La cámara puede moverse en todas direcciones mediante un sistema de centro-objetivo, pero para rotarla en ciertas direcciones depende de un vector de referencia.
    * El viewport no es donde se ven los píxeles, es simplemente un marco de referencia para los vectores salientes.
    * Hacer figuras complejas es EXAGERADAMENTE difícil.
    * Debido a cómo funciona el renderizado de figuras planas, tanto los paralelogramos como los triángulos están en el mismo archivo. Si en algún momento esto resulta contraproducente, se puede modificar para que queden como archivos separados, puesto que meramente están juntos por una cuestión de claridad.
    * La optimización implementada (bvh), si bien es un tanto compleja, a la hora de paralelizar o de generar entradas no es relevante, por lo que no es necesario entender cómo funciona.
    * Una vez esté compilado el código, se pueden guardar los resultados en un archivo .ppm mediante, por ejemplo: main.exe > image.ppm
    * Hay una carpeta la cual tiene imágenes varias del proceso de creación.

- Tareas:
    * Formato para almacenar objetos de forma externa al código.
    * Paralelizar secciones de imagen mediante OpenMP.
    * Paralelizar generacios de vectores aleatorios por pixel mediante OpenMP.
    * Hacer antialiasing en máster.
    * Instancias de rotación para facilitación de puesta de objetos en escena.

- Modificaciones importantes respecto a libro:
    * No se ha implementado antialiasing, para simplificar paralelización.
    * Ciertas partes del código han sido alteradas para mayor claridad.
    * Se ha añadido la clase triángulo.
    * El método de difusión de luz no va mediante texturas, ya que estas no se han implementado.
