rem gcc test.c gradient.c gradient_descent.c loss_function.c matrix.c two_layer_net.c
rem gcc test.c gradient.c loss_function.c matrix.c two_layer_net.c
rem gcc gradient_descent.c gradient.c loss_function.c matrix.c two_layer_net.c teacher_file.c
rem gcc  gradient.c loss_function.c matrix.c two_layer_net.c teacher_file.c network_data.c -o 
gcc  deep_learning.c gradient_descent.c gradient.c loss_function.c matrix.c two_layer_net.c teacher_file.c network_data.c 
gcc  create_teacher_data.c gradient.c loss_function.c matrix.c two_layer_net.c teacher_file.c network_data.c -o create_teacher_data.exe
gcc  network_test.c gradient.c loss_function.c matrix.c two_layer_net.c teacher_file.c network_data.c -o network_test.exe
