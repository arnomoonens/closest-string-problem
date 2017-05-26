
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
upb <- optimals[,"ub"]

mmas.results.file <- paste0(results.folder, "/mmas.txt")
acs.results.file <- paste0(results.folder, "/acs.txt")

mmas.results <- read.csv(mmas.results.file, header = FALSE)
colnames(mmas.results) <- c("Instance", 1:10)
rownames(mmas.results) <- mmas.results$Instance
mmas.results$Instance <- NULL

acs.results <- read.csv(acs.results.file, header = FALSE)
colnames(acs.results) <- c("Instance", 1:10)
rownames(acs.results) <- acs.results$Instance
acs.results$Instance <- NULL

# Best (B), worst (W), mean (M) and standard deviation (SD) for both algorithms
mmas.stats <- data.frame(
  B=apply(mmas.results, 1, min),
  W=apply(mmas.results, 1, max),
  M=apply(mmas.results, 1, mean),
  SD=apply(mmas.results, 1, sd)
  )

acs.stats <- data.frame(
  B=apply(acs.results, 1, min),
  W=apply(acs.results, 1, max),
  M=apply(acs.results, 1, mean),
  SD=apply(acs.results, 1, sd)
)

rpd <- function(x) ((x - upb) * 100) / upb

# Relative percentage deviation (between mean and upper best) for both algorithms
rpd(mmas.stats[,"M"])
rpd(acs.stats[,"M"])

# Wilcoxon test
wilcox.test(mmas.stats[,"M"], acs.stats[,"M"])
