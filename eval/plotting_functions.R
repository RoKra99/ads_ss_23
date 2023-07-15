plot <- function(data, xaxis, yaxis, color_vec) {
    #print(data)
  # This plots the mean and standard error for each section, and connects the points with lines
  ggplot(data, aes(x = xaxis, y = yaxis, group = algo, colour = algo)) +
      #scale_x_continuous(trans='log2') +
      scale_colour_manual(values=color_vec, limits = force, drop=TRUE) +
      stat_summary(fun.data = mean_se, geom = 'pointrange') +
      stat_summary(fun = mean, geom = 'line')
}

plot_log <- function(data, xaxis, yaxis, color_vec) {
    #print(data)
  # This plots the mean and standard error for each section, and connects the points with lines
  ggplot(data, aes(x = xaxis, y = yaxis, group = algo, colour = algo)) +
      scale_y_continuous(trans='log10') +
      annotation_logticks(sides="l") +
      scale_colour_manual(values=color_vec, limits = force, drop=TRUE) +
      stat_summary(fun.data = mean_se, geom = 'pointrange') +
      stat_summary(fun = mean, geom = 'line')
}

plot_bar_percent <- function(data, fillv, xaxis , yaxis) {
    ggplot(dt1, aes(fill=fillv, y=yaxis, x=factor(xaxis))) + 
    geom_bar(position="fill", stat="identity")+
    facet_wrap(~d_graph, strip.position = "bottom")
}

plot_box_grpuped <- function(data, fillv, xaxis, yaxis){
 ggplot(dt1, aes(fill=fillv, y=yaxis, x=factor(xaxis))) + 
    geom_boxplot()+
    facet_wrap(~d_graph, strip.position = "bottom")+
    scale_y_continuous(trans='log10', limits=c(1,NA)) +
    annotation_logticks(sides="l") 

}
