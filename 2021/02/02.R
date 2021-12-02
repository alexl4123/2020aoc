# 02.12.2021
data <- read.csv("02/ex_2_02.csv", sep = ",", header = FALSE)


ex_1 <- function(data) {
  sol <- Reduce(f = function(cum, i) {
      if (data[i,1] == "forward") {
        cum[1] <- cum[1] + data[i,2]
      } else if (data[i,1] == "down") {
        cum[2] <- cum[2] + data[i,2]
      } else {
        cum[2] <- cum[2] - data[i,2]
      }
      # print(paste(cum[1], cum[2]))
      cum <- cum
    }, x = c(1:length(data[,1])), init = c(0,0))
  return (sol)
}
sol1 <- ex_1(data)
sol1 <- sol1[1] * sol1[2]

ex_2 <- function(data) { 
  sol <- Reduce(f = function(cum, i) {
      if (data[i,1] == "forward") {
        cum[1] <- cum[1] + data[i,2]
        cum[2] <- cum[2] + (cum[3] * data[i,2])
      } else if (data[i,1] == "down") {
        cum[3] <- cum[3] + data[i,2]
      } else {
        cum[3] <- cum[3] - data[i,2]
      }
      # print(paste(cum[1], cum[2]))
      cum <- cum
    }, x = c(1:length(data[,1])), init = c(0,0,0))
  return (sol)
} 
sol2 <- ex_2(data)
sol2 <- sol2[1] * sol2[2]











