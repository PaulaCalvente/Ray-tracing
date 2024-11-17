# Ray-tracing

- Detalles a tener en cuenta:
    * Ni idea de si el código funciona tal cual, pero si no es el caso debes seguir las instrucciones del inicio de: https://raytracing.github.io/books/RayTracingInOneWeekend.html
    * Colores no implementados, la coloración sólo es un reflejo de las normales de la superficie (vamos que es mera coincidencia que el suelo sea verde).
    * El suelo es una esfera, un poco ridículo lo sé.
    * Todo está más o menos comentado excepto main.cpp y camera.h, ya que son los que parecen tener más cambios a futuro.
    * La cámara sí puede moverse pero no puede realizar giros de ningún tipo.
    * Si os da error algún archivo en el editor es un problema de este, el código es completamente funcional debido a las herencias de clases. Igualmente se podrían incluir las clases para que no dé error, pero realmente es innecesario.

- Cambios respecto a código anterior:
    * Archivos añadidos: 
    * Archivos modificados: 

- Tareas:
    * Intentar simplificar hittable_list.h.
    * Ver posibilidad de implementar planos de perimetros limitados.
    * Formato para almacenar objetos de forma externa al código.
    * Paralelizar secciones de imagen mediante OpenMP.
    * Hacer antialiasing en master.

- Modificaciones importantes respecto a libro:
    * La clase del libro "rtweekend.h" ha sido cambiada a "header.h" para una mayor comprensión.
    * No se ha implementado antialiasing, para simplificar paralelización.