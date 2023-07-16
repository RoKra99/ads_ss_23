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
plot(df, df$input, df$time, color_vec_pd) + labs(title = 'Cumulative Running Time for  1M Queries',  x = 'n [Million]', y = 'time [ms] ', color = 'Algorithm')
plot_log(df, df$input, df$time, color_vec_pd) + labs(title = 'Cumulative Running Time for  1M  Queries (Logplot)',  x = 'n in Million', y = 'time [ms] (log)', color = 'Algorithm')

plot(df, df$input, df$constr, color_vec_pd) + labs(title = 'Construction Time',  x = 'n  [Million]', y = 'time [ms] ', color = 'Algorithm')
plot_log(df, df$input, df$constr, color_vec_pd) + labs(title = 'Construction Time (Logplot)',  x = 'n in Million', y = 'time [ms] (log)', color = 'Algorithm')

plot(df, df$input, df$memory / df$input / 1000000, color_vec_pd) + labs(title = 'Speicherplatzverbrauch',  x = 'n  [Million]', y = 'memory [bit/Zahl] ', color = 'Algorithm')
plot_log(df, df$input, df$memory / df$input / 1000000, color_vec_pd) + labs(title = 'Speicherplatzverbrauch (Logplot)',  x = 'n  [Million]', y = 'memory [bit/number] (log) ', color = 'Algorithm')

df = data_pd[(data_pd$input == 8),]
plot(df, df$max, df$time, color_vec_pd) + labs(title = 'Memory relative to maximmum Input (1 Million Queries, n= 8 Million) ',  x = 'Largest Input (log)', y = 'time [ms] ', color = 'Algorithm')
plot(df, df$max, df$memory / 8000000, color_vec_pd) + labs(title = 'Memory relative to maximmum Input (1 Million Queries, n = 8 Million) ',  x = 'Largest Input (log)', y = 'memory [bit/number] ', color = 'Algorithm')


# RMQ

df = data_rmq[(data_rmq$max == 64),]
print(df)
plot(df, df$input, df$time, color_vec_rmq) + labs(title = 'Cumulative Running Time for  10 Million Queries',  x = 'n [Million]', y = 'time [ms] ', color = 'Algorithm')

plot(df, df$input, df$constr, color_vec_rmq) + labs(title = 'Construction Time',  x = 'n  [Million]', y = 'time [ms] ', color = 'Algorithm')
plot_log(df, df$input, df$constr, color_vec_rmq) + labs(title = 'Construction Time (Logplot)',  x = 'n in Million', y = 'time [ms] (log)', color = 'Algorithm')

plot(df, df$input, df$memory / df$input / 1000000, color_vec_rmq) + labs(title = 'Memory Usage',  x = 'n  [Million]', y = 'memory [bit/number] ', color = 'Algorithm')
plot_log(df, df$input, df$memory / df$input / 1000000, color_vec_rmq) + labs(title = 'Memory Usage (Logplot)',  x = 'n  [Million]', y = 'memory [bit/number] (log) ', color = 'Algorithm')


# Closes the PDF
dev.off()
