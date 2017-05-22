
# Uncomment to use in RStudio
commandArgs <- function (trailingOnly=FALSE) {
  c("/Users/arnomoonens/Dropbox/MA2-AI/Swarm_Intelligence/project/results", "/Users/arnomoonens/Dropbox/MA2-AI/Swarm_Intelligence/project/instances/instances_opt.txt")
}

args = commandArgs(trailingOnly=TRUE)
if (length(args)<2) {
  stop("2 arguments needed. Usage: Rscript analysis.R PATH/TO/RESULTS/ PATH/TO/instances_opt.txt", call.=FALSE)
}

results.folder <- args[1]
optimals <- read.table(file=args[2], header=TRUE, sep=";") 

first.results.file <- paste0(results.folder, "/first.txt")
acs.results.file <- paste0(results.folder, "/acs.txt")

first.results <- read.csv(first.results.file, header = FALSE)
colnames(first.results) <- c("Instance", 1:10)
rownames(first.results) <- first.results$Instance
first.results$Instance <- NULL

cs.results <- read.csv(acs.results.file, header = FALSE)
colnames(acs.results) <- c("Instance", 1:10)
rownames(acs.results) <- acs.results$Instance
acs.results$Instance <- NULL

# Best (B), worst (W), mean (M) and standard deviation (SD) for both algorithms
first.stats <- data.frame(
  B=apply(first.results, 1, min),
  W=apply(first.results, 1, max),
  M=apply(first.results, 1, mean),
  SD=apply(first.results, 1, sd)
  )

acs.stats <- data.frame(
  B=apply(acs.results, 1, min),
  W=apply(acs.results, 1, max),
  M=apply(acs.results, 1, mean),
  SD=apply(acs.results, 1, sd)
)

rpd <- function(x) ((x - upb) * 100) / upb

# Wilcoxon test
wilcox.test(first.stats[,"M"], acs.stats[,"M"])
