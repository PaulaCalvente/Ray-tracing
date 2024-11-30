# Ray-tracing

- Cosas a tener en cuenta:
    * Si Dios está de tu lado, deberías poder compilarlo con: g++ -fopenmp -o main main.cpp.
    * Una vez esté compilado el código, se pueden guardar los resultados en un archivo .txt mediante, por ejemplo: main.exe > image.txt.
    * El .txt se puede convertir en un .ppm mediante el uso de parser_image.py.
    * GIMP puede abrir la imagen, así que te tocará abandonar el camino del bien e instalarlo.
    * Si el código no funciona tal cual (gracioso lo sé), debes seguir las instrucciones del inicio de: https://raytracing.github.io/books/RayTracingInOneWeekend.html.
    * El número de hilos está puesto para que detecte el máximo de hilos físicos del procesador, pero esto se puede modificar manualmente para que se aproveche la memoria RAM y así tardar un poco menos (a costa de que el procesador se prenda fuego).
    * No hay un suelo, por lo que se debe crear mediante un objeto extra, un poco ridículo lo sé.
    * La cámara puede moverse en todas direcciones mediante un sistema de centro-objetivo, pero para rotarla en ciertas direcciones depende de un vetor de referencia.
    * El viewport no es donde se ven los píxeles, es simplemente un marco de referencia para lo vectores salientes.
    * Debido a cómo funciona el renderizado de figuras planas, tanto los paralelogramos como los triángulos están en el mismo archivo. Si en algún momento esto resulta contraproducente, se puede modificar para que queden como archivos separados, puesto que meramente están juntos por una cuestión de claridad.
    * Hay una carpeta la cual tiene imágenes varias del proceso de creación.

- Tareas:
    * Formato para almacenar objetos de forma externa al código.
    * Paralelizar secciones de imagen mediante OpenMP.
    * Paralelizar generacios de vectores aleatorios por pixel mediante OpenMP.
    * Hacer antialiasing en máster.
    * Instancias de rotación para facilitación de puesta de objetos en escena.

- Modificaciones importantes respecto a libro:
    * La información de la imagen se guarda en un .txt con las coordenadas y el color de los píxeles para posteriormente ser procesada por parser_image.py, el cual retorna un .ppm.
    * No se ha implementado antialiasing, para simplificar paralelización.
    * Ciertas partes del código han sido alteradas para mayor claridad.
    * Se ha añadido la clase triángulo.
    * El método de difusión de luz no va mediante texturas, ya que estas no se han implementado.
