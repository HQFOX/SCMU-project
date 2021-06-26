package com.example.plantmonitor.adapter

import android.content.Context
import android.content.Intent
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.example.plantmonitor.PlantDetailsActivity
import com.example.plantmonitor.R
import com.example.plantmonitor.model.Plant

class ItemAdapter(private val context: Context, private val dataset: List<Plant>) : RecyclerView.Adapter<ItemAdapter.ItemViewHolder>() {

    class ItemViewHolder(val view: View) : RecyclerView.ViewHolder(view) {
        val button: TextView = view.findViewById(R.id.button_item)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ItemViewHolder {
        // create a new view
        val adapterLayout = LayoutInflater.from(parent.context)
            .inflate(R.layout.list_item, parent, false)

        return ItemViewHolder(adapterLayout)

    }

    override fun onBindViewHolder(holder: ItemViewHolder, position: Int) {
        val item = dataset[position]
        holder.button.text = item.toString()

        holder.button.setOnClickListener{

            val context = holder.view.context

            val intent = Intent(context, PlantDetailsActivity::class.java)

            intent.putExtra("plant", holder.button.text.toString())

            context.startActivity(intent)
        }
    }

    override fun getItemCount(): Int = dataset.size

}
