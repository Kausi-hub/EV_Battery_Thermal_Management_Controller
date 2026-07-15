import streamlit as st
import pandas as pd
import plotly.express as px
import plotly.graph_objects as go

# ------------------------------------------------------------------
# Page Config
# ------------------------------------------------------------------

st.set_page_config(
    page_title="EV Thermal Dashboard",
    page_icon="🚗",
    layout="wide"
)

# ------------------------------------------------------------------
# Load Data
# ------------------------------------------------------------------

@st.cache_data(ttl=2)
def load_data():

    df = pd.read_csv("telemetry.csv")

    df["TimeSec"] = (
        df["TimeMs"] / 1000.0
    )

    return df


df = load_data()

# ------------------------------------------------------------------
# KPIs
# ------------------------------------------------------------------

current_temp = df["Temp1"].iloc[-1]

current_coolant = df["CoolantTemp"].iloc[-1]

max_temp = df["Temp1"].max()

max_coolant = df["CoolantTemp"].max()

peak_current = df["Current"].max()

fault_count = (df["Fault"] > 0).sum()

thermal_margin = (
    55.0 - current_temp
)

cooling_efficiency = (
    current_temp -
    current_coolant
)

avg_pump = df["PumpCmd"].mean()

avg_fan = df["FanCmd"].mean()

fault_free_runtime = (
    (df["Fault"] == 0).sum()
    /
    len(df)
) * 100

control_error = abs(
    current_temp - 35.0
)

# ------------------------------------------------------------------
# Status
# ------------------------------------------------------------------

if current_temp < 40:
    thermal_status = "🟢 NORMAL"

elif current_temp < 50:
    thermal_status = "🟠 WARNING"

else:
    thermal_status = "🔴 CRITICAL"

cooling_health = (
    "✅ HEALTHY"
    if (
        df["PumpRpm"].max() > 3000
        and
        df["FanRpm"].max() > 2000
    )
    else "❌ FAULT"
)

# ------------------------------------------------------------------
# Header
# ------------------------------------------------------------------

st.title(
    "🚗 EV Battery Thermal Management Dashboard"
)

st.markdown(
    f"### Thermal Status: {thermal_status}"
)

# ------------------------------------------------------------------
# KPI ROW 1
# ------------------------------------------------------------------

c1, c2, c3, c4 = st.columns(4)

c1.metric(
    "Peak Battery Temp",
    f"{max_temp:.2f} °C"
)

c2.metric(
    "Peak Coolant Temp",
    f"{max_coolant:.2f} °C"
)

c3.metric(
    "Peak Current",
    f"{peak_current:.0f} A"
)

c4.metric(
    "Fault Events",
    int(fault_count)
)

# ------------------------------------------------------------------
# KPI ROW 2
# ------------------------------------------------------------------

c5, c6, c7, c8 = st.columns(4)

c5.metric(
    "Thermal Margin",
    f"{thermal_margin:.2f} °C"
)

c6.metric(
    "Cooling Efficiency",
    f"{cooling_efficiency:.2f} °C"
)

c7.metric(
    "Pump Utilization",
    f"{avg_pump:.1f}%"
)

c8.metric(
    "Fan Utilization",
    f"{avg_fan:.1f}%"
)

# ------------------------------------------------------------------
# KPI ROW 3
# ------------------------------------------------------------------

c9, c10, c11 = st.columns(3)

c9.metric(
    "Fault Free Runtime",
    f"{fault_free_runtime:.1f}%"
)

c10.metric(
    "Control Error",
    f"{control_error:.4f} °C"
)

c11.metric(
    "Cooling Health",
    cooling_health
)

# ------------------------------------------------------------------
# Temperature Chart
# ------------------------------------------------------------------

st.header(
    "Battery vs Coolant Temperatures"
)

temp_fig = go.Figure()

temp_fig.add_trace(
    go.Scatter(
        x=df["TimeSec"],
        y=df["Temp1"],
        name="Battery Temp"
    )
)

temp_fig.add_trace(
    go.Scatter(
        x=df["TimeSec"],
        y=df["CoolantTemp"],
        name="Coolant Temp"
    )
)

st.plotly_chart(
    temp_fig,
    use_container_width=True
)

# ------------------------------------------------------------------
# Commands
# ------------------------------------------------------------------

st.header(
    "Cooling Commands"
)

cmd_fig = go.Figure()

cmd_fig.add_trace(
    go.Scatter(
        x=df["TimeSec"],
        y=df["PumpCmd"],
        name="Pump Cmd"
    )
)

cmd_fig.add_trace(
    go.Scatter(
        x=df["TimeSec"],
        y=df["FanCmd"],
        name="Fan Cmd"
    )
)

st.plotly_chart(
    cmd_fig,
    use_container_width=True
)

# ------------------------------------------------------------------
# RPM Feedback
# ------------------------------------------------------------------

st.header(
    "Pump / Fan Feedback"
)

rpm_fig = go.Figure()

rpm_fig.add_trace(
    go.Scatter(
        x=df["TimeSec"],
        y=df["PumpRpm"],
        name="Pump RPM"
    )
)

rpm_fig.add_trace(
    go.Scatter(
        x=df["TimeSec"],
        y=df["FanRpm"],
        name="Fan RPM"
    )
)

st.plotly_chart(
    rpm_fig,
    use_container_width=True
)

# ------------------------------------------------------------------
# Current vs Temperature
# ------------------------------------------------------------------

st.header(
    "Current vs Temperature"
)

scatter_fig = px.scatter(
    df,
    x="Current",
    y="Temp1",
    color="PumpCmd"
)

st.plotly_chart(
    scatter_fig,
    use_container_width=True
)

# ------------------------------------------------------------------
# Fault Timeline
# ------------------------------------------------------------------

st.header(
    "Fault Timeline"
)

faults = df[
    df["Fault"] > 0
]

if len(faults):

    fault_fig = px.scatter(
        faults,
        x="TimeSec",
        y="Fault",
        size="Fault",
        color="Fault"
    )

    st.plotly_chart(
        fault_fig,
        use_container_width=True
    )

# ------------------------------------------------------------------
# Fault Distribution
# ------------------------------------------------------------------

if len(faults):

    st.header(
        "Fault Distribution"
    )

    fault_dist = (
        faults.groupby("Fault")
        .size()
        .reset_index(name="Count")
    )

    pie = px.pie(
        fault_dist,
        names="Fault",
        values="Count"
    )

    st.plotly_chart(
        pie,
        use_container_width=True
    )

# ------------------------------------------------------------------
# Validation Report
# ------------------------------------------------------------------

st.header(
    "Validation Report"
)

requirements = [

    (
        "Peak Temp < 55°C",
        max_temp < 55
    ),

    (
        "Pump RPM > 3000",
        df["PumpRpm"].max() > 3000
    ),

    (
        "Fan RPM > 2000",
        df["FanRpm"].max() > 2000
    ),

    (
        "Telemetry Samples >= 600",
        len(df) >= 600
    ),

    (
        "Control Error < 5°C",
        control_error < 5
    )
]

for name, result in requirements:

    if result:
        st.success(f"{name}: PASS")
    else:
        st.error(f"{name}: FAIL")

# ------------------------------------------------------------------
# Raw Data
# ------------------------------------------------------------------

with st.expander("Telemetry Data"):

    st.dataframe(df.tail(100))