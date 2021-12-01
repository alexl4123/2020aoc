data <- read.csv("01/ex_2_01.csv", sep = ",", header = FALSE)
data <- unlist(data)

# 1. exercise:

first_exercise <- function(data) {
  cum <- 0
  prev <- data[1]
  for (i in c(2:length(data))) {
    if (data[i] > prev) {
      cum <- cum + 1
    }
    prev <- data[i]
  }
  return (cum)
}
sol_1 <- first_exercise(data)


# 2. exercise:

new_data <- c()
for (i in c(3:length(data))) {
  sum <- data[i] + data[i-1] + data[i-2]
  new_data <- c(new_data, sum)
}

data_2 <- first_exercise(new_data)











