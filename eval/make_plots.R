# Loads libraries
library(tidyverse)
library(dplyr)
source("../eval/plotting_functions.R")

# Path to the result files
pd_folder='../pd/results/'

############################ PD ##############################################
# Different versions of the algorithm we want to plot
algos_pd = c('manual', 'x_fast_trie', 'y_fast_trie')
algo_pd_names = c('manual', 'xfast', 'yfast')
n = length(algos_pd)
color_vec_pd = setNames(hcl(seq(15,375,length=n+1)[1:n], 100, 65), algo_pd_names)

# Reads the files and creates the union of all rows
data_pd <- algos_pd %>% map(function(a) {
  # The name of the file with the results
  file <- paste(pd_folder, a, '.txt', sep='')
  # Reads the data
  raw <- read.table(file, comment.char = '#', col.names = c('algo', 'input', 'max', 'memory', 'constr', 'it', 'time'))

}) %>% reduce(union_all)

# Path to the result files
rmq_folder='../rmq/results/'

############################ RMQ ##############################################
# Different versions of the algorithm we want to plot
algos_rmq = c('linear', 'nlogn')
algo_rmq_names = c('linear', 'nlogn')
n = length(algos_rmq)
color_vec_rmq = setNames(hcl(seq(15,375,length=n+1)[1:n], 100, 65), algo_rmq_names)

# Reads the files and creates the union of all rows
data_rmq <- algos_rmq %>% map(function(a) {
  # The name of the file with the results
  file <- paste(rmq_folder, a, '.txt', sep='')
  # Reads the data
  raw <- read.table(file, comment.char = '#', col.names = c('algo', 'input', 'max', 'memory', 'constr', 'it', 'time'))

}) %>% reduce(union_all)



############################ PLOTTING ##############################################
# Opens a PDF to store the plots
pdf("plots.pdf", width=10, height=5)
options(scipen=999)

# PD 
df = data_pd[(data_pd$max == 64),]
plot(df, df$input, df$time, color_vec_pd) + labs(title = 'Laufzeit 1Mio Queries ',  x = 'Eingabegröße [Mio]', y = 'Zeit [ms] ', color = 'Algorithm')
plot_log(df, df$input, df$time, color_vec_pd) + labs(title = 'Laufzeit 1Mio Queries (Logplot)',  x = 'Eingabegröße in Mio', y = 'Zeit [ms] (log)', color = 'Algorithm')

plot(df, df$input, df$constr, color_vec_pd) + labs(title = 'Initialisierungszeit',  x = 'Eingabegröße  [Mio]', y = 'Zeit [ms] ', color = 'Algorithm')
plot_log(df, df$input, df$constr, color_vec_pd) + labs(title = 'Initialisierungszeit (Logplot)',  x = 'Eingabegröße in Mio', y = 'Zeit [ms] (log)', color = 'Algorithm')

plot(df, df$input, df$memory / 8000000, color_vec_pd) + labs(title = 'Speicherplatzverbrauch',  x = 'Eingabegröße  [Mio]', y = 'Speicherplatz [bit/Zahl] ', color = 'Algorithm')
plot_log(df, df$input, df$memory / 8000000, color_vec_pd) + labs(title = 'Speicherplatzverbrauch (Logplot)',  x = 'Eingabegröße  [Mio]', y = 'Speicherplatz [bit/Zahl] (log) ', color = 'Algorithm')

df = data_pd[(data_pd$input == 8),]
plot(df, df$max, df$time, color_vec_pd) + labs(title = 'Laufzeit abhängig von maximaler Eingabe (1 Mio Queries, 8 Mio Eingabewerte) ',  x = 'Max Eingabe (log)', y = 'Zeit [ms] ', color = 'Algorithm')
plot(df, df$max, df$memory / 8000000, color_vec_pd) + labs(title = 'Speicherplatz abhängig von maximaler Eingabe (1 Mio Queries, 8 Mio Eingabewerte) ',  x = 'Max Eingabe (log)', y = 'Speicherplatz [bit] ', color = 'Algorithm')


# RMQ

df = data_rmq[(data_rmq$max == 64),]
print(df)
plot(df, df$input, df$time, color_vec_rmq) + labs(title = 'Laufzeit 10Mio Queries ',  x = 'Eingabegröße [Mio]', y = 'Zeit [ms] ', color = 'Algorithm')

plot(df, df$input, df$constr, color_vec_rmq) + labs(title = 'Initialisierungszeit',  x = 'Eingabegröße  [Mio]', y = 'Zeit [ms] ', color = 'Algorithm')
plot_log(df, df$input, df$constr, color_vec_rmq) + labs(title = 'Initialisierungszeit (Logplot)',  x = 'Eingabegröße in Mio', y = 'Zeit [ms] (log)', color = 'Algorithm')

plot(df, df$input, df$memory / 8000000, color_vec_rmq) + labs(title = 'Speicherplatzverbrauch',  x = 'Eingabegröße  [Mio]', y = 'Speicherplatz [bit/Zahl] ', color = 'Algorithm')
plot_log(df, df$input, df$memory / 8000000, color_vec_rmq) + labs(title = 'Speicherplatzverbrauch (Logplot)',  x = 'Eingabegröße  [Mio]', y = 'Speicherplatz [bit/Zahl] (log) ', color = 'Algorithm')


# Closes the PDF
dev.off()
