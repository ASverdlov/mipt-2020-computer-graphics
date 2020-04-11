# Репозиторий для приема заданий по компьютерной графике // ФИВТ МФТИ 2019 // Семинарист Федоров А.А.
## План занятий и сдач
№ | Дата | Тема
-:|-------|------------------|
1 | 08 февраля | [GL-0: Принцип организации работы GPU. GPGPU. Compute Shaders. Query Object. Shared Memory.](https://paper.dropbox.com/doc/GL-0-GPU.-GPGPU.-Compute-Shaders.-Query-Object.-Shared-Memory.--AuhScAltZdTZrCrudrcsjHR0AQ-07Mp3JQYv4Xp01cQdq0rJ)
2 | 15 февраля | GL-0; [GL-1: OpenGL pipeline](https://paper.dropbox.com/doc/GL-1-OpenGL-pipeline--AuhMs_QlYBVKD2CdWvAJDHGUAQ-esCO87TYKGLujgajb6Mk4);
- | 22 февраля | Неучебный день - олимпиада МФТИ
3 | 29 февраля | [GL-2: Interpolation, shaders, attributes, uniforms](https://paper.dropbox.com/doc/GL-2-Interpolation-shaders-attributes-uniforms--AvbQmRJ1GHGie9LdS_koozaqAQ-Mrx0ULAF9rkatB3apEAnf)
4 | 7 марта | [GL-4: Texturing](https://paper.dropbox.com/doc/GL-4-Texturing--Awh1EJlyjfWg9Zj2YOSg9XDIAQ-SywTp5W1FC5K9jr7NneKJ)
5 | 14 марта | Приём задания №1
6 | 21 марта | GL-4: Texturing (продолжение)
7 | 28 марта | каникулы
8 | 4 апреля | [GL-3: Lighting](https://paper.dropbox.com/doc/GL-3-Lighting--Axa78hkaT08FCVKSNF1hDkUUAQ-XRJ12Uba4C5MgHVGfMdJC)  [GL-5: Depth buffer, viewport, stencil, blending](https://paper.dropbox.com/doc/GL-5-Depth-buffer-viewport-stencil-blending.--AxZ5MEZhMKk_sm1cr8InGJ3gAQ-HU8yEOY0vkpXzbfoiMR3U)
9 | 11 апреля | [GL-6: Framebuffer, MRT, Shadows](https://paper.dropbox.com/doc/GL-6-Framebuffer-MRT-shadows--Ax6JNsBkUjSSyEtB0tzJA~5PAQ-xm2K9FAYZj96L9JHc8Xch)
10 | 18 апреля | GL-?мы очень опаздываем; GL-8; GL-9
11 | 25 апреля | Приём задания №2
11,5 | 2 мая | GL-?
- | 9 мая | День Победы
12 | 16 мая | GL-11
13 | 23+ мая | Прием задания №3

## Краткое руководство по встраиванию

* Создайте форк этого репозитория.
* Для каждого задания выделена отдельная папка (*task1*, *task2*).
* В папке с заданием создайте свою папку `<номер группы><фамилия на латинице>` (например, *123Ivanov*), работайте только в этой папке.
* Создайте вложенную подпапку `<номер группы><фамилия на латинице>Data<номер задания>` (например, *123IvanovData1*). Используйте эту папку для размещения загружаемых в программе файлов (3D модели, изображения и т.д.).
* В папке `<номер группы><фамилия на латинице>` создайте файл CMakeLists.txt следующего содержимого

```
set(SRC_FILES
    Main.h
    Main.cpp
)

MAKE_OPENGL_TASK(123Ivanov 1 "${SRC_FILES}")
```

Здесь в переменной **SRC_FILES** укажите имена ваших файлов с исходным кодом.
    
В аргументах макроса **MAKE_OPENGL_TASK** укажите имя папки и номер задания (1, 2 или 3).

### Образец
В репозитории приведены примеры: задание 2 по OpenGL (скопированы из примеров к курсу).

### Зависимости
Не рекомендуется инклюдить файлы из примеров оформления задания: они общие, вдруг вам понадобится что-то поменять.
Скопируйте всё, что вам нужно.